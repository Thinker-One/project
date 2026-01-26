#ifndef UDEV_COMMON_TYPES_H
#define UDEV_COMMON_TYPES_H
#include "common.hpp"

class UsbDevice;

namespace UsbCommonTyps {

    // common function obj
    using add_remove_udev_cb = std::function<void(std::shared_ptr<UsbDevice>)>;
    using print_udev_info_cb = std::function<void(const std::shared_ptr<UsbDevice> &)>;
    using print_udevs_info_cb = std::function<void()>;
    using get_udev_size = std::function<int()>;

    // common struct
    struct UsbDeviceInfo {
        std::string syspath;        // /sys/devices/...
        std::string sysname;
        std::string sysnum;
        std::string subsystem;      // usb
        std::string devpath;
        std::string devnode;        // /dev/bus/usb/001/002 (可能为空)
        std::string devtype;        // usb_device / usb_interface
        std::string action;         // add/remove/change（监听时有）
        std::string driver;

        std::string product;
        std::string vendor_id;      // VID
        std::string product_id;     // PID
        std::string manufacturer;
        std::string serial;

        int busnum = -1;
        int devnum = -1;
    };

    struct Callbacks {
        add_remove_udev_cb add_udev_cb;
        add_remove_udev_cb remove_udev_cb;
        print_udev_info_cb p_udev_info_cb;
        print_udevs_info_cb p_udevs_info_cb;
        get_udev_size get_udev_size_cb;
    };

}

#endif