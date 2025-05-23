#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <cstring>
#include <errno.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <thread>
#include <arpa/inet.h>
#include <regex>
#include <termios.h>

#include "log.h"

typedef enum {
    HUB_PKG_TYPE_EVE,
    HUB_PKG_TYPE_METHOD_REQ,
    HUB_PKG_TYPE_METHOD_REP,
    HUB_PKG_TYPE_MAX
}uHubPkgType_t;

#pragma pack(1)
struct Head {
    uint32_t headFlag;              // 头标识 4
    uint16_t protoVersion;          // 协议版本 2
    uint8_t msgType;                // 消息类型 1
    uint32_t payloadLen;            // payload 长度 4
    uint8_t checkSum;               // payload 校验和 1
    uint32_t reserve;               // 预留字段 4
};
#pragma pack()

#endif