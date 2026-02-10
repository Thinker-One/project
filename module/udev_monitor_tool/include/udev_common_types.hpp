#ifndef UDEV_COMMON_TYPES_H
#define UDEV_COMMON_TYPES_H

#include <memory>
#include <unordered_map>
#include <libudev.h>

class UsbDevice;
class UsbInterface;

namespace UsbCommonTyps {

    struct DevBaseInfo {
        std::string    syspath;             // /sys/devices/...
        std::string    sysname;
        std::string    subsystem;
        std::string    devpath;
        std::string    devtype;
        std::string    devname;
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
        std::string    removable;
        std::string    maxchild;

        int busnum;
        int devnum;

        UsbDeviceInfo() : busnum(-1), devnum(-1) {}
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

    struct DeviceNum {
        // 总数量
        int num_total = 0;

        // Communication 类 (CDC, Class 0x02)
        int num_type_virtualcomport = 0;
        int num_type_netcard = 0;

        // HID 类 (0x03) - 人机交互设备
        int num_type_keyboard = 0;
        int num_type_mouse = 0;

        // 打印机 (Class 0x07)
        int num_type_printer = 0;

        // Mass Storage 类 (0x08) - 存储设备
        int num_type_optical = 0;
        int num_type_floppy = 0;
        int num_type_disk = 0;

        // 自定义类（0xFF）
        int num_type_custom = 0;

        // 未知类型
        int num_type_unknown = 0;
    };
    
    struct InterfaceDescriptor {
        std::string interface_class;
        std::string interface_subclass;
        std::string interface_protocol;
    };



    // variable type
    using UsbDeviceMap = std::unordered_map<std::string, std::shared_ptr<UsbDevice>>;           // syspath -> UsbDevice
    using UsbInterfaceMap = std::unordered_map<std::string, std::shared_ptr<UsbInterface>>;     // syspath -> UsbInterface
    using UsbDeviceMapPtr = std::shared_ptr<UsbDeviceMap>;
    using UsbInterfaceMapPtr = std::shared_ptr<UsbInterfaceMap>;



    // Polymorphic Function Wrapper
    using add_remove_usb_dev_cb = std::function<void(std::shared_ptr<UsbDevice>)>;
    using print_one_usb_dev_info_cb = std::function<void(const std::shared_ptr<UsbDevice> &)>;
    using print_all_usb_dev_info_cb = std::function<void()>;
    using print_one_interface_info_cb = std::function<void(const std::shared_ptr<UsbInterface> &)>;
    using print_all_interface_info_cb = std::function<void()>;
    using get_usb_dev_num_cb = std::function<int()>;
    using get_usb_interface_num_cb = std::function<int()>;
    using get_devices_ptr_cb = std::function<UsbDeviceMapPtr()>;
    using get_num_of_various_device_cb = std::function<std::shared_ptr<DeviceNum>()>;



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



    // callbacks
    struct Callbacks {
        add_remove_usb_dev_cb               add_usb_dev_cb;
        add_remove_usb_dev_cb               remove_usb_dev_cb;
        print_one_usb_dev_info_cb           p_usb_dev_info_cb;
        print_all_usb_dev_info_cb           p_usb_devs_info_cb;
        print_one_interface_info_cb         p_interface_info_cb;
        print_all_interface_info_cb         p_interfaces_info_cb;
        get_usb_dev_num_cb                  get_usb_dev_number_cb;
        get_usb_interface_num_cb            get_usb_interface_number_cb;
        get_devices_ptr_cb                  get_devs_ptr_cb;
        get_num_of_various_device_cb        get_num_of_various_dev_cb;

        Callbacks ()
        {
            add_usb_dev_cb                  = nullptr;
            remove_usb_dev_cb               = nullptr;
            p_usb_dev_info_cb               = nullptr;
            p_usb_devs_info_cb              = nullptr;
            p_interface_info_cb             = nullptr;
            p_interfaces_info_cb            = nullptr;
            get_usb_dev_number_cb           = nullptr;
            get_usb_interface_number_cb     = nullptr;
            get_devs_ptr_cb                 = nullptr;
            get_num_of_various_dev_cb       = nullptr;
        }

        Callbacks (
            add_remove_usb_dev_cb           cb1,
            add_remove_usb_dev_cb           cb2,
            print_one_usb_dev_info_cb       cb3,
            print_all_usb_dev_info_cb       cb4,
            print_one_interface_info_cb     cb5,
            print_all_interface_info_cb     cb6,
            get_usb_dev_num_cb              cb7,
            get_usb_interface_num_cb        cb8,
            get_devices_ptr_cb              cb9,
            get_num_of_various_device_cb    cb10
        )
        {
            add_usb_dev_cb                  = cb1;
            remove_usb_dev_cb               = cb2;
            p_usb_dev_info_cb               = cb3;
            p_usb_devs_info_cb              = cb4;
            p_interface_info_cb             = cb5;
            p_interfaces_info_cb            = cb6;
            get_usb_dev_number_cb           = cb7;
            get_usb_interface_number_cb     = cb8;
            get_devs_ptr_cb                 = cb9;
            get_num_of_various_dev_cb       = cb10;
        }
    };

}

#endif