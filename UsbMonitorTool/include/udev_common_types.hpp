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
        std::string syspath_;        // /sys/devices/...
        std::string devnode_;        // /dev/bus/usb/001/002 (可能为空)
        std::string action_;         // add/remove/change（监听时有）

        std::string subsystem_;      // usb
        std::string devtype_;        // usb_device / usb_interface

        std::string vendor_id_;      // VID
        std::string product_id_;     // PID
        std::string manufacturer_;
        std::string product_;
        std::string serial_;

        int busnum_ = -1;
        int devnum_ = -1;
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