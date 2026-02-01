#ifndef UDEV_COMMON_TYPES_H
#define UDEV_COMMON_TYPES_H
#include <libudev.h>
#include "common.hpp"

class UsbDevice;

namespace UsbCommonTyps {

    // variable type
    using UsbDeviceMap = std::unordered_map<std::string, std::shared_ptr<UsbDevice>>;   // syspath -> UsbDevice
    using UsbDeviceMapPtr = std::shared_ptr<UsbDeviceMap>;



    // Polymorphic Function Wrapper
    using add_remove_usb_dev_cb = std::function<void(std::shared_ptr<UsbDevice>)>;
    using print_one_usb_dev_info_cb = std::function<void(const std::shared_ptr<UsbDevice> &)>;
    using print_all_usb_dev_info_cb = std::function<void()>;
    using get_usb_dev_num_cb = std::function<int()>;
    using get_devices_ptr_cb = std::function<UsbDeviceMapPtr()>;



    // functor
    template <typename T, T* (*UdevUnref)(T*)>
    struct UdevFuncDel {
        void operator()(T *p) const {
            if (p) UdevUnref(p);
        }
    };

    using UdevDel = UdevFuncDel<struct udev, udev_unref>;
    using UdevMonDel = UdevFuncDel<struct udev_monitor, udev_monitor_unref>;
    using UdevEnumerateDel = UdevFuncDel<struct udev_enumerate, udev_enumerate_unref>;
    using UdevDeviceDel = UdevFuncDel<struct udev_device, udev_device_unref>;

    using UdevDelPtr = std::unique_ptr<struct udev, UdevDel>;
    using UdevMonDelPtr = std::unique_ptr<struct udev_monitor, UdevMonDel>;
    using UdevEnumerateDelPtr = std::unique_ptr<struct udev_enumerate, UdevEnumerateDel>;
    using UdevDeviceDelPtr = std::unique_ptr<struct udev_device, UdevDeviceDel>;



    // common struct
    struct DevBaseInfo {
        std::string    syspath;             // /sys/devices/...
        std::string    sysname;
        std::string    subsystem;
        std::string    devpath;
        std::string    devtype;
        std::string    driver;
    };

    struct UsbDeviceInfo {
        DevBaseInfo    base_info;
        std::string    sysnum;
        std::string    devnode;             // /dev/bus/usb/001/002 (可能为空)
        std::string    action;              // add/remove/change（监听时有）
        std::string    product;
        std::string    vendor_id;           // VID
        std::string    product_id;          // PID
        std::string    manufacturer;
        std::string    serial;

        int busnum = -1;
        int devnum = -1;
    };

    struct UsbInterfaceInfo {
        DevBaseInfo    base_info;
        std::string    authorized;           
        std::string    bAlternateSetting;         
        std::string    bInterfaceClass;            
        std::string    bInterfaceNumber;           
        std::string    bInterfaceProtocol;          
        std::string    bInterfaceSubClass;        
        std::string    bNumEndpoints;      
        std::string    modalias;      
        std::string    supports_autosuspend;
        std::string    uevent;
    };

    struct Callbacks {
        add_remove_usb_dev_cb       add_usb_dev_cb;
        add_remove_usb_dev_cb       remove_usb_dev_cb;
        print_one_usb_dev_info_cb   p_usb_dev_info_cb;
        print_all_usb_dev_info_cb   p_usb_devs_info_cb;
        get_usb_dev_num_cb          get_usb_dev_number_cb;
        get_devices_ptr_cb          get_devs_ptr_cb;
    };

}

#endif