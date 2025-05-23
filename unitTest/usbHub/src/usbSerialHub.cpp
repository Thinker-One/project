#include "../include/usbSerialHub.h"

UsbSerialHub::UsbSerialHub() {

}

UsbSerialHub::~UsbSerialHub() {

}

int UsbSerialHub::getFdOfSerial(std::string devicePath) {
    hubFd = open(devicePath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (hubFd < 0) {
        LOG(RED, "无法打开设备: %s", strerror(errno));
        return -1;
    }

    // 配置串口
    struct termios tty;
    if (tcgetattr(hubFd, &tty) != 0) {
        std::cerr << "无法获取串口属性" << std::endl;
        close(hubFd);
        return 1;
    }

    // 设置波特率 
    cfsetispeed(&tty, B115200); // 输入波特率
    cfsetospeed(&tty, B115200); // 输出波特率

    // 配置其他参数
    tty.c_cflag &= ~PARENB; // 无奇偶校验
    tty.c_cflag &= ~CSTOPB; // 1 位停止位
    tty.c_cflag &= ~CSIZE;  // 清除数据位掩码
    tty.c_cflag |= CS8;     // 8 位数据位
    tty.c_cflag &= ~CRTSCTS; // 禁用硬件流控
    tty.c_cflag |= CREAD | CLOCAL; // 启用接收，忽略控制线

    tty.c_lflag &= ~ICANON; // 禁用规范模式
    tty.c_lflag &= ~ECHO;   // 禁用回显
    tty.c_lflag &= ~ECHOE;  // 禁用擦除
    tty.c_lflag &= ~ECHONL; // 禁用换行回显
    tty.c_lflag &= ~ISIG;   // 禁用信号字符

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // 禁用软件流控
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    tty.c_oflag &= ~OPOST; // 禁用输出处理
    tty.c_oflag &= ~ONLCR;

    // 应用配置
    if (tcsetattr(hubFd, TCSANOW, &tty) != 0) {
        std::cerr << "无法设置串口属性" << std::endl;
        close(hubFd);
        return 1;
    }

    return 0;
}

void UsbSerialHub::reciveData() {

    // 设置文件描述符为非阻塞模式（可选）
    int flags = fcntl(hubFd, F_GETFL, 0);
    fcntl(hubFd, F_SETFL, flags | ~O_NONBLOCK);

    // 定义 select 的文件描述符集
    fd_set read_fds, write_fds;
    struct timeval timeout;

    while (true) {
        // 清空文件描述符集
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);

        // 将设备文件描述符加入读集合
        FD_SET(hubFd, &read_fds);
        FD_SET(hubFd, &write_fds);

        // 设置超时时间（2秒超时）
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        // 调用 select 监听事件
        int ret = select(hubFd + 1, &read_fds, NULL, nullptr, &timeout);
        if (ret < 0) {
            std::cerr << "select 错误: " << strerror(errno) << std::endl;
            break;
        }

        // 检查是否有数据可读
        if (FD_ISSET(hubFd, &read_fds)) {
            unsigned char buffer[256] = {0};
            int bytes_read = read(hubFd, buffer, sizeof(buffer));
            if (bytes_read > 0) {
                std::cout << "收到数据, buff size: " << bytes_read << std::endl;
                for (int i = 0; i < bytes_read; ++i) {
                    std::cout << buffer[i];
                }
                std::cout << std::endl;
            } else {
                std::cerr << "读取错误: " << strerror(errno) << std::endl;
            }
        }

    }

    close(hubFd);
}

int UsbSerialHub::handleShakeRep() {

    return 0;
}

int UsbSerialHub::lightingReq(int index, std::string color) {

    // set body
    nlohmann::ordered_json lightReqJson = {
        {"msgId", 12345},
        {"methodName", "ledCtl"},
        {"payload", {
            {"ledList", {
                {{"ledIndex", index}, {"color", color}}
            }}
        }}
    };
    std::string lightReqJsonStr = lightReqJson.dump();

    // set head
    uint8_t sum = 0;
    for (uint32_t i = 0; i < lightReqJsonStr.size(); i++) {
        sum ^= lightReqJsonStr[i];
    }
    Head packHead = {
        .headFlag = htonl(0xAA554342),
        .protoVersion = htons(0x0001),
        .msgType = uHubPkgType_t::HUB_PKG_TYPE_METHOD_REQ,
        .payloadLen = htonl(lightReqJsonStr.size()),
        .checkSum = sum,
        .reserve = 0x00000000
    };

    // send req to hub
    LOG(GREEN, "开始点灯: index:0, color:%s",color.c_str());
    sendto(lightReqJsonStr, packHead);
    // sendtoBody(lightReqJsonStr);
    return 0;
}

bool UsbSerialHub::sendto(std::string& reqStr, Head& packHead) {

    int buffSize = sizeof(Head)+reqStr.size(), pos = 0, ret = 0;
    std::unique_ptr<char[]> buffPtr = std::make_unique<char[]>(buffSize);
    std::fill_n(buffPtr.get(), buffSize, 0);
    memcpy(buffPtr.get(), &packHead, sizeof(Head));
    memcpy(buffPtr.get()+sizeof(Head), reqStr.c_str(), reqStr.size());
    ret = write(hubFd, buffPtr.get()+pos, buffSize);
    if ( ret == -1) {
        LOG(RED, "write faild, error: %s", strerror(errno));
        return false;
    } else {
        LOG(DEFAULT, "Send buff success, buff size: %d", ret);
    }
    return true;
}

bool UsbSerialHub::sendtoBody(std::string& reqStr) {
    int buffSize = reqStr.size(), pos = 0, ret = 0;
    std::unique_ptr<char[]> buffPtr = std::make_unique<char[]>(buffSize);
    std::fill_n(buffPtr.get(), buffSize, 0);
    memcpy(buffPtr.get(), reqStr.c_str(), reqStr.size());
    while (buffSize) {
        ret = write(hubFd, buffPtr.get()+pos, 64);
        pos += 64;
        buffSize -= 64;
        if ( ret == -1) {
            LOG(RED, "write faild, error: %s", strerror(errno));
            return false;
        } else {
            LOG(DEFAULT, "Send buff success, buff size: %d", ret);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return true;
}

void UsbSerialHub::test() {
    this->getFdOfSerial("/dev/ttyUSB0");
    // select revice data from hub
    std::unique_ptr<std::thread> reciveDataThreadPtr;
    reciveDataThreadPtr = std::make_unique<std::thread>(&UsbSerialHub::reciveData, this);
    // reciveDataThreadPtr->join();

    // 握手
    this->handleShakeRep();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 点灯
    // for (auto color : {"red"}) {
    //     this->lightingReq(0, color);
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    // }

    for (auto color : {"red", "green", "blue", "yellow", "purple", "none"}) {
        this->lightingReq(0, color);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    LOG(YELLOW, "结束点灯");
    reciveDataThreadPtr->join();
}