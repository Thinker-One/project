#include "usb_manager.hpp"

int main() {
    UsbManager& manager = UsbManager::get_instance();
    manager.monitor_start();
    // while (true) {
    //     std::this_thread::sleep_for(std::chrono::seconds(3));
    // }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    manager.monitor_stop();
    return 0;
}