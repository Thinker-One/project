#include <sys/stat.h>
#include <errno.h>
#include <spdlog.hpp>


namespace CommonTool {
    void create_dir_recurise(const std::string& path) {
        if (path.empty()) return;

        std::string dir = path;

        // 去掉末尾 '/'
        while (dir.size() > 1 && dir.back() == '/') {
            dir.pop_back();
        }

        size_t pos = 0;
        while (true) {
            pos = dir.find('/', pos + 1);
            std::string subdir = dir.substr(0, pos);

            if (subdir.empty()) continue;

            if (mkdir(subdir.c_str(), 0755) != 0) {
                if (errno == EEXIST) {
                    struct stat st;
                    if (stat(subdir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
                        LOG_ERROR("Path exists but is not a directory: {}", subdir.c_str());
                        return;
                    }
                } else {
                    LOG_ERROR("mkdir failed: {}, errno={} ({})", subdir.c_str(), errno, strerror(errno));
                    return;
                }
            }

            if (pos == std::string::npos) break;
        }
    }

    PathInfo analyze_path(const std::string& path) {
        PathInfo info = {false, "", PathType::NOT_FOUND};
        struct stat st;

        // 1. 使用 lstat 检查路径本身是否是软链接
        // lstat 不会自动追踪链接，而 stat 会直接获取目标的信息
        if (lstat(path.c_str(), &st) != 0) {
            LOG_WARN("路径不存在或无权访问, {}", path);
            return info; // 路径不存在或无权访问
        }

        if (S_ISLNK(st.st_mode)) {
            info.is_symlink = true;
            
            // 2. 获取真实路径 (处理多级跳转和相对路径)
            char resolved_path[PATH_MAX];
            if (realpath(path.c_str(), resolved_path) != nullptr) {
                info.real_path = resolved_path;
                
                // 3. 再次判断真实路径指向的是什么
                struct stat real_st;
                if (stat(info.real_path.c_str(), &real_st) == 0) {
                    info.type = get_type_from_stat(real_st);
                }
            } else {
                // realpath 失败，可能是“断链”（指向的文件被删了）
                info.real_path = "BROKEN_LINK";
            }
        } else {
            // 不是软链接，直接获取类型
            info.is_symlink = false;
            info.real_path = path;
            info.type = get_type_from_stat(st);
        }

        return info;
    }

    PathType get_type_from_stat(const struct stat& st) {
        if (S_ISREG(st.st_mode)) return PathType::REGULAR_FILE;
        if (S_ISDIR(st.st_mode)) return PathType::DIRECTORY;
        return PathType::OTHER;
    }
}
