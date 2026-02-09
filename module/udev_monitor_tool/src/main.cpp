#include "usb_device_manager.hpp"

int main() {
    try {
        UsbDeviceManager& manager = UsbDeviceManager::get_instance();
        manager.start();
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        manager.stop();
    } catch (const std::exception& e) {
        LOG_ERROR("main error={}", e.what());
    }
    return 0;
}