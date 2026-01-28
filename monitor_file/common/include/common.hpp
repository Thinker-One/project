#ifndef COMMON_HPP
#define COMMON_HPP
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <chrono>
#include <spdlog.hpp>
#include <string.h>
#include <functional>


namespace CommonTool {
    
    enum class PathType {
        NOT_FOUND,
        REGULAR_FILE,
        DIRECTORY,
        OTHER // 如管道、设备文件等
    };

    struct PathInfo {
        bool is_symlink;
        std::string real_path;
        PathType type;
    };

    void create_dir_recurise(const std::string& path);
    PathInfo analyze_path(const std::string& path);
    PathType get_type_from_stat(const struct stat& st);
}

#endif