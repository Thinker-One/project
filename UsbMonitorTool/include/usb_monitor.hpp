#ifndef USB_MONITOR_HPP
#define USB_MONITOR_HPP
#include <thread>
#include <mutex>
#include <atomic>
#include <libudev.h>
#include <poll.h>
#include "common.hpp"
#include "usb_device.hpp"
#include "udev_common_types.hpp"

class UsbMonitor {

public:
    UsbMonitor();
    ~UsbMonitor();

public:
    int init();
    int start();
    int stop();
    static std::shared_ptr<UsbDevice> parse_udev(struct udev_device *udev);
    void set_callback (UsbCommonTyps::Callbacks &cbs);
    void listen_usb_hotplug();

private:
    UsbDevice udev_;
    std::shared_ptr<std::thread> monitor_thread_ptr_;
    std::atomic_bool running_;
    UsbCommonTyps::Callbacks callbacks_;
};

#endif