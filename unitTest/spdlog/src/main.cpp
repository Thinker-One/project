#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Welcome to spdlog!");  // 信息级别日志
    spdlog::error("Something went wrong!");  // 错误级别日志
    spdlog::warn("Warning: This is a warning.");  // 警告级别日志
}
