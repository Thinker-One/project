#include <iostream>
#include <cstdio>
#include <string>

#define ENABLE_LOG 1		                       // 控制打印的开启和关闭
#if ENABLE_LOG
#define LOG(color, fmt, ...) \
    do { \
        std::cout << color; \
        printf(fmt, ##__VA_ARGS__); \
        std::cout << DEFAULT << std::endl; \
    } while (0)
#else
#define LOG(color, fmt, ...)
#endif

const std::string DEFAULT = "\033[0m";                 // 重置颜色
const std::string RED     = "\033[38;2;255;100;100m";  // 亮红色 (RGB: 255, 100, 100)
const std::string GREEN   = "\033[38;2;100;255;100m";  // 亮绿色 (RGB: 100, 255, 100)
const std::string YELLOW  = "\033[38;2;255;255;100m";  // 亮黄色 (RGB: 255, 255, 100)
const std::string BLUE    = "\033[38;2;100;100;255m";  // 亮蓝色 (RGB: 100, 100, 255)
const std::string MAGENTA = "\033[38;2;255;100;255m";  // 亮品红色 (RGB: 255, 100, 255)
const std::string CYAN    = "\033[38;2;100;255;255m";  // 亮青色 (RGB: 100, 255, 255)
const std::string WHITE   = "\033[38;2;255;255;255m";  // 亮白色 (RGB: 255, 255, 255)