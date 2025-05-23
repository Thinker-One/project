#include "../include/usbHidHub.h"
#include "../include/usbSerialHub.h"


int main(int argc, char *argv[]) {
    if (argc != 2 || (strcmp(argv[1], "serial") != 0 && strcmp(argv[1], "hid") != 0)) {
        std::cout << "Usage: " << argv[0] << " <serial|hid>" << std::endl;
        return  -1;
    }
    if (strcmp(argv[1], "serial") == 0) {
        std::shared_ptr<UsbSerialHub> serialHubPtr = std::make_shared<UsbSerialHub>();
        serialHubPtr->test();
    } else {
        std::shared_ptr<UsbHidHub> hidHubPtr = std::make_shared<UsbHidHub>();
        hidHubPtr->test();
    }
    return 0;
}

