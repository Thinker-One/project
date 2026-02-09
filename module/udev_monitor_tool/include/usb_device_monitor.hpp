#ifndef USB_DEVICE_MONITOR_HPP
#define USB_DEVICE_MONITOR_HPP
#include <thread>
#include <mutex>
#include <atomic>
#include <libudev.h>
#include <poll.h>
#include "common.hpp"
#include "usb_device.hpp"
#include "udev_common_types.hpp"

class UsbDeviceMonitor {

public:
    UsbDeviceMonitor();
    ~UsbDeviceMonitor();

public:
    int start();
    int stop();
    static std::shared_ptr<UsbDevice> parse_usb_device(UsbCommonTyps::UsbDeviceMapPtr devs, struct udev_device *udev);
    static std::shared_ptr<UsbDevice> parse_usb_interface (UsbCommonTyps::UsbDeviceMapPtr devs, struct udev_device *udev);
    void set_callback (UsbCommonTyps::Callbacks &cbs);
    void listen_usb_device_hotplug();

private:
    UsbDevice usb_dev_;
    std::shared_ptr<std::thread> usb_dev_mon_thd_ptr_;
    std::atomic<bool> running_;
    UsbCommonTyps::Callbacks callbacks_;
    int timeout_ms_;
};

#endif