#ifndef USB_DEVICE_HPP
#define USB_DEVICE_HPP

#include <vector>
#include <memory>
#include "udev_common_types.hpp"
#include "usb_interface.hpp"

class UsbDevice {

public:
    UsbDevice();
    ~UsbDevice();

public:
    enum class UsbDeviceState {
        INIT,
        UNINIT
    };

public:
    void set_usb_dev_state(UsbDeviceState state);
    void set_syspath(std::string syspath);
    void set_sysname(std::string sysname);
    void set_sysnum(std::string sysnum);
    void set_subsystem(std::string subsystem);
    void set_devpath(std::string devpath);
    void set_devname(UsbCommonTyps::InterfaceDescriptor &describe, std::shared_ptr<UsbCommonTyps::DeviceNum> num);
    void set_devnode(std::string devnode);
    void set_devtype(std::string devtype);
    void set_action(std::string action);
    void set_driver(std::string driver);
    void set_product(std::string product);
    void set_vendor_id(std::string vendor_id);
    void set_product_id(std::string product_id);
    void set_manufacturer(std::string manufacturer);
    void set_serial(std::string serial);
    void set_busnum(int busnum);
    void set_devnum(int devnum);
    void set_removable(std::string removable);
    void set_maxchild(std::string maxchild);
    void set_latest_interface(std::shared_ptr<UsbInterface> &interface);

public:
    UsbDeviceState get_usb_dev_state();
    std::string get_syspath();
    std::string get_sysname();
    std::string get_sysnum();
    std::string get_subsystem();
    std::string get_devpath();
    std::string get_devnode();
    std::string get_devtype();
    std::string get_action();
    std::string get_driver();
    std::string get_product();
    std::string get_vendor_id();
    std::string get_product_id();
    std::string get_manufacturer();
    std::string get_serial();
    int get_busnum();
    int get_devnum();
    std::string get_removable();
    std::string get_maxchild();
    std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> get_usb_device_info();
    std::vector<std::shared_ptr<UsbCommonTyps::UsbInterfaceInfo>> get_usb_interfaces_info();
    int get_usb_interface_number();
    std::shared_ptr<UsbInterface> get_latest_interface();

public:
    UsbCommonTyps::UsbInterfaceMap interfaces_;
    
private:
    UsbDeviceState state_;
    std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> usb_dev_info_ptr_;
    std::shared_ptr<UsbInterface> latest_interface_ptr_;

};

#endif