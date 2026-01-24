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
}
