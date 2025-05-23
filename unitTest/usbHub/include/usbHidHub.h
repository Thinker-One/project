#ifndef USB_HID_HUB_H
#define USB_HID_HUB_H

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../../common/common.h"

class UsbHidHub {

public:
    UsbHidHub();
    ~UsbHidHub();

public:
    void test();
    int getFdOfHID(std::string devicePath);
    void reciveData();
    int handleShakeReq();
    int lightingReq(int index, std::string color);
    bool sendto(std::string& reqStr, Head& packHead);
    bool sendtoBody(std::string& reqStr);

private:
    int hubFd;
    
};

#endif