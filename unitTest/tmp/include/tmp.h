#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/hidraw.h>
#include <cstring>
#include <errno.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <thread>
#include <arpa/inet.h>
#include <linux/hidraw.h>
#include <chrono>
#include <ctime>
#include <libudev.h>

#include "../../common/log.h"

