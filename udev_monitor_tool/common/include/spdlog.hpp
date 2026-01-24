#ifndef SPDLOG_HPP
#define SPDLOG_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "common.hpp"

#define LOG_FILE_DIR "/var/log/usb_monitor"
#define LOG_FILE_PATH LOG_FILE_DIR "/usb_monitor.log"
#define LOG_FILE_MAX_SIZE (5 * 1024 * 1024)
#define LOG_FILE_MAX_COUNT  3



namespace logger {

    inline void create_dir_recurise(const std::string& path) {
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
                        // fprintf(stderr, "Path exists but is not a directory: %s\n", subdir.c_str());
                        // LOG_ERROR("Path exists but is not a directory: {}", subdir.c_str());
                        return;
                    }
                } else {
                    // fprintf(stderr, "mkdir failed: %s, errno=%d (%s)\n", subdir.c_str(), errno, strerror(errno));
                    // LOG_ERROR("mkdir failed: {}, errno={} ({})", subdir.c_str(), errno, strerror(errno));
                    return;
                }
            }

            if (pos == std::string::npos) break;
        }
    }

    inline void init_once() {
        static bool initialized = []() {
            try {
                std::vector<spdlog::sink_ptr> sinks;
        #if STDOUT_ENABLED
                // 控制台（彩色）
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                sinks.push_back(console_sink);
        #endif

        #if LOG_TO_FILE_ENABLED
                // 滚动文件
                create_dir_recurise(LOG_FILE_DIR);
                auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                        LOG_FILE_PATH,
                        LOG_FILE_MAX_SIZE,
                        LOG_FILE_MAX_COUNT
                );
                sinks.push_back(file_sink);
        #endif

                // 组合logger
                auto logger = std::make_shared<spdlog::logger>("usb_monitor", sinks.begin(), sinks.end());

                // 日志级别
                logger->set_level(spdlog::level::debug);
                spdlog::set_default_logger(logger);

                // 日志格式
                logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%!:%#] %v");

                // 刷盘策略
                spdlog::flush_on(spdlog::level::warn);
                spdlog::flush_every(std::chrono::seconds(3));

            } catch (const spdlog::spdlog_ex& ex) {
                fprintf(stderr, "spdlog init failed: %s\n", ex.what());
            }

            return true;
        }();

        (void)initialized;
    }

}


#define LOG_TRACE(...)      do { logger::init_once(); SPDLOG_TRACE(__VA_ARGS__);    }   while (0)
#define LOG_DEBUG(...)      do { logger::init_once(); SPDLOG_DEBUG(__VA_ARGS__);    }   while (0)
#define LOG_INFO(...)       do { logger::init_once(); SPDLOG_INFO(__VA_ARGS__);     }   while (0)
#define LOG_WARN(...)       do { logger::init_once(); SPDLOG_WARN(__VA_ARGS__);     }   while (0)
#define LOG_ERROR(...)      do { logger::init_once(); SPDLOG_ERROR(__VA_ARGS__);    }   while (0)
#define LOG_CRITICAL(...)   do { logger::init_once(); SPDLOG_CRITICAL(__VA_ARGS__); }   while (0)


#endif