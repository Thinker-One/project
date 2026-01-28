#include "spdlog.hpp"

#if SPDLOG_ACTIVE_LEVEL > SPDLOG_LEVEL_DEBUG
#warning "SPDLOG_ACTIVE_LEVEL > DEBUG, SPDLOG_DEBUG will be compiled out"
#endif