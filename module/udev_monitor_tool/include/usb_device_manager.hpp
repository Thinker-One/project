#ifndef USB_DEVICE_MANAGER_HPP
#define USB_DEVICE_MANAGER_HPP
#include "common.hpp"
#include "usb_device_monitor.hpp"
#include "usb_device_observer.hpp"
#include "udev_common_types.hpp"

class UsbDeviceManager {

public:
    UsbDeviceManager(const UsbDeviceManager&) = delete;
    UsbDeviceManager& operator=(const UsbDeviceManager&) = delete;

public:
    static UsbDeviceManager& get_instance();
    int start();
    int stop();
    

private:
    void set_callback();
    bool enumerate_usb_device();
    int monitor_start();
    int monitor_stop();
    void add_usb_device(std::shared_ptr<UsbDevice> usb_dev);
    void remove_usb_device(std::shared_ptr<UsbDevice> usb_dev);
    void print_all_usb_device_info();
    void print_all_usb_interface_info();
    void print_new_usb_device_info(const std::shared_ptr<UsbDevice> &usb_dev);
    void print_new_interface_info(const std::shared_ptr<UsbInterface> &usb_dev);
    int get_usb_device_total_num();
    UsbCommonTyps::DeviceNum get_num_of_various_dev();
    int get_usb_interface_number();
    static UsbCommonTyps::UsbDeviceMapPtr get_devs_ptr();

    
private:
    UsbDeviceManager();
    ~UsbDeviceManager();

private:
    std::shared_ptr<UsbDeviceMonitor> mon_ptr_;
    std::vector<std::unique_ptr<UsbDeviceObserver>> observers_;
    static UsbCommonTyps::UsbDeviceMapPtr devs_ptr_;
    static UsbCommonTyps::DeviceNum num_of_various_dev_;
    std::vector<std::shared_ptr<UsbDevice>> devs_type_disk_;
    UsbCommonTyps::Callbacks callbacks_;
};

#endif