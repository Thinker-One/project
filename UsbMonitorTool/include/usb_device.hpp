#ifndef USB_DEVICE_HPP
#define USB_DEVICE_HPP
#include "common.hpp"
#include "udev_common_types.hpp"

class UsbDevice {

public:
    UsbDevice();
    ~UsbDevice();

public:
    void set_syspath(std::string syspath);
    void set_devnode(std::string devnode);
    void set_action(std::string action);
    void set_subsystem(std::string subsystem);
    void set_devtype(std::string devtype);
    void set_vendor_id(std::string vendor_id);
    void set_product_id(std::string product_id);
    void set_manufacturer(std::string manufacturer);
    void set_product(std::string product);
    void set_serial(std::string serial);
    void set_busnum(int busnum);
    void set_devnum(int devnum);

    std::string get_syspath();
    std::string get_devnode();
    std::string get_action();
    std::string get_subsystem();
    std::string get_devtype();
    std::string get_vendor_id();
    std::string get_product_id();
    std::string get_manufacturer();
    std::string get_product();
    std::string get_serial();
    int get_busnum();
    int get_devnum();

    std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> get_all_udevs_info();

private:
    int init();
    
private:
    std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> udev_info_ptr_;
};

#endif