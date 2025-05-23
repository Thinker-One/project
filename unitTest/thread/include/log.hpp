#pragma once
#include <spdlog/spdlog.h>

struct LoggerInitializer {
    LoggerInitializer() {
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%!:%#] %v");
    }
};

namespace {
    LoggerInitializer _logger_init;  // 静态初始化
}

#define LOG_INFO(...)    SPDLOG_LOGGER_INFO(spdlog::default_logger(), __VA_ARGS__)
#define LOG_DEBUG(...)   SPDLOG_LOGGER_DEBUG(spdlog::default_logger(), __VA_ARGS__)
#define LOG_WARN(...)    SPDLOG_LOGGER_WARN(spdlog::default_logger(), __VA_ARGS__)
#define LOG_ERROR(...)   SPDLOG_LOGGER_ERROR(spdlog::default_logger(), __VA_ARGS__)
