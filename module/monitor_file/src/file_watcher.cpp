#include <sys/inotify.h>
#include <spdlog.hpp>
#include <pwd.h>
#include "file_watcher.hpp"

FileWatcher::FileWatcher() :
    inotify_fd_(-1),
    running_(false),
    last_trigger_time_(std::chrono::steady_clock::now()) 
{}

FileWatcher::~FileWatcher() {
    stop();
}

bool FileWatcher::init() {
    if (running_) return false;
    running_ = true;
    dirs_.clear();
    watch_descs_.clear();
    already_inotify_dirs_.clear();

    inotify_fd_ = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
    if (inotify_fd_ < 0) {
        LOG_ERROR("inotify_init1 failed.");
        return false;
    }

    get_inotify_dir();
    if (dirs_.empty()) {
        close(inotify_fd_);
        inotify_fd_ = -1;
        running_ = false;
        return false;
    }

    for (auto &it : dirs_) {
        if (!already_inotify_dirs_.count(it.first)) {
            already_inotify_dirs_.emplace(it.first);
        } else {
            continue;
        }
        LOG_INFO("新增监控路径, DIR = {}", it.first);
        int wd = inotify_add_watch (
            inotify_fd_,
            it.first.c_str(),
            IN_CLOSE_WRITE | IN_MOVED_TO | IN_MOVED_FROM | IN_CREATE | IN_DELETE | IN_ATTRIB
        );

        if (wd < 0) {
            LOG_ERROR("inotify_add_watch failed, dir={}", it.first);
            continue;
        }
        watch_descs_.emplace(wd, it.first);
    }

    return true;
}

int FileWatcher::start() {
    // print_inotify_dir_info();
    watch_thread_ = std::thread(&FileWatcher::run, this);
    return 0;
}

int FileWatcher::stop() {
    if (!running_.exchange(false)) return  -1;

    if (watch_thread_.joinable()) {
        watch_thread_.join();
    }

    if (inotify_fd_ >= 0) {
        close(inotify_fd_);
        inotify_fd_ = -1;
    }
    return 0;
}

void FileWatcher::split_path(const std::string& full, std::string& dir, std::string& file) {
    auto pos = full.find_last_of('/');
    if (pos == std::string::npos) {
        dir = ".";
        file = full;
    } else {
        dir = full.substr(0, pos);
        file = full.substr(pos + 1);
    }
}

bool FileWatcher::is_vim_noise(const std::string& filename) {
    if (filename.empty()) return false;

    if (filename.find_first_not_of("0123456789") == std::string::npos) {
        return true;
    }

    if (filename.back() == '~') {
        return true;
    }

    if (filename.size() > 4 && filename.front() == '.') {
        size_t last_dot = filename.find_last_of('.');
        if (last_dot != std::string::npos && last_dot > 0) {
            std::string ext = filename.substr(last_dot);
            if (ext.size() >= 4 && ext.compare(0, 3, ".sw") == 0) {
                return true;
            }
        }
    }

    return false;
}

void FileWatcher::run() {

    char buffer[MAX_BUFFER_SIZE];

    while (running_) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(inotify_fd_, &rfds);

        timeval tv{1, 0};
        int ret = select(inotify_fd_ + 1, &rfds, nullptr, nullptr, &tv);
        if (ret <= 0) continue;

        ssize_t len = read(inotify_fd_, buffer, sizeof(buffer));
        if (len < 0) {
            if (errno == EAGAIN || errno == EINTR) continue;
            break;
        }

        auto now = std::chrono::steady_clock::now();

        for (ssize_t i = 0; i < len; ) {
            auto* ev = reinterpret_cast<inotify_event*>(&buffer[i]);
            if (ev->len && !is_vim_noise(ev->name) && is_monitor_obj(watch_descs_[ev->wd] ,ev->name)) {
                if (ev->mask & (IN_CLOSE_WRITE | IN_MOVED_TO | IN_MOVED_FROM | IN_CREATE | IN_DELETE)) {
                    handle_event(now, ev->name);
                }
            }
            i += sizeof(inotify_event) + ev->len;
        }
    }
}

bool FileWatcher::is_monitor_obj(std::string dir, std::string filename) {
    if (dir == "/etc" && filename != "crontab" && filename != "rc.local") {
        return false;
    } else if ((dir.substr(0, 6) == "/home/" || dir.substr(0, 6) == "/root") && \
                filename != ".bashrc" && filename != ".bash_profile" && \
                filename != ".profile" && filename != ".cshrc")
    {
        return false;
    }
    return true;
}

void FileWatcher::handle_event(std::chrono::steady_clock::time_point now, const std::string &str) {

    std::lock_guard<std::mutex> lock(mutex_);
    if (now - last_trigger_time_ < std::chrono::milliseconds(500)) {
        return;
    }

    last_trigger_time_ = now;
    LOG_WARN("文件变动:={}", str);
}

void FileWatcher::get_inotify_dir() {
    std::vector<std::string> dirs = {
        "/etc",
        "/etc/crontab",
        "/etc/cron.d",
        "/var/spool/cron/crontabs",
        "/etc/init.d",
        "/etc/rc.d",
        "/etc/rc0.d",
        "/etc/rc1.d",
        "/etc/rc2.d",
        "/etc/rc3.d",
        "/etc/rc4.d",
        "/etc/rc5.d",
        "/etc/rc6.d",
        "/etc/rc.local",
        "/etc/rc.d/rc0.d",
        "/etc/rc.d/rc1.d",
        "/etc/rc.d/rc2.d",
        "/etc/rc.d/rc3.d",
        "/etc/rc.d/rc4.d",
        "/etc/rc.d/rc5.d",
        "/etc/rc.d/rc6.d",
        "/etc/rc.d/init.d"
    };
    
    for (auto &d : dirs) {
        CommonTool::PathInfo info = CommonTool::analyze_path(d);
        if (info.type == CommonTool::PathType::REGULAR_FILE) {
            std::pair<std::string, std::string> new_dir;
            split_path(info.real_path, new_dir.first, new_dir.second);
            bool found = false;
            for (auto &it : dirs_) {
                if (it.first == new_dir.first && it.second == new_dir.second) {
                    found = true;
                    break;
                }
            }
            if (!found) dirs_.push_back(new_dir);
        } else if (info.type == CommonTool::PathType::DIRECTORY) {
            bool found = false;
            for (auto &it : dirs_) {
                if (it.first == info.real_path) {
                    found = true;
                    break;
                }
            }
            if (!found) dirs_.push_back({info.real_path, ""});
        }
    }

    get_user_startup_dirs();
}

void FileWatcher::print_inotify_dir_info() {
    for (auto &it : dirs_) {
        LOG_INFO("dir={}, filename={}", it.first, it.second);
    }
}

void FileWatcher::get_user_startup_dirs() {

    std::vector<std::pair<std::string, std::string>> dirs;
    setpwent();  // 打开 passwd 数据库
    struct passwd* pw;

    const std::vector<std::string> startup_files = {
        ".bashrc",
        ".bash_profile",
        ".profile",
        ".cshrc"
    };

    while ((pw = getpwent()) != nullptr) {
        // 过滤系统用户（按需调整）
        if (pw->pw_uid < 1000 && pw->pw_uid > 0)
            continue;

        if (!pw->pw_dir || pw->pw_dir[0] == '\0')
            continue;

        // 检查 home 是否存在
        std::string home = pw->pw_dir;
        struct stat st;
        if (stat(pw->pw_dir, &st) != 0 || !S_ISDIR(st.st_mode))
            continue;

        bool is_exsit = false;
        for (const auto& f : startup_files) {
            std::string path = home + "/" + f;
            if (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                is_exsit = true;
                dirs_.push_back({home, f});
            }
        }

        if (!is_exsit) dirs_.push_back({home, ""});
    }

    endpwent();
}
