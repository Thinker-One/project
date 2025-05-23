#ifndef USB_SERIAL_HUB_H
#define USB_SERIAL_HUB_H

#include "../../common/common.h"

class UsbSerialHub {

public:
    UsbSerialHub();
    ~UsbSerialHub();


public:
    void test();
    int getFdOfSerial(std::string devicePath);
    void reciveData();
    int handleShakeRep();
    int lightingReq(int index, std::string color);
    bool sendto(std::string& reqStr, Head& packHead);
    bool sendtoBody(std::string& reqStr);

private:
    int hubFd;

};

#endif