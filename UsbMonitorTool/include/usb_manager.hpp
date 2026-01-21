#ifndef USB_MANAGER_HPP
#define USB_MANAGER_HPP
#include "common.hpp"
#include "usb_monitor.hpp"
#include "usb_observer.hpp"
#include "udev_common_types.hpp"

class UsbManager {

public:
    UsbManager(const UsbManager&) = delete;
    UsbManager& operator=(const UsbManager&) = delete;

public:
    static UsbManager& get_instance();
    int monitor_start();
    int monitor_stop();

private:
    void init();
    void set_callback();
    bool enumerate_udevs();
    void add_udev(std::shared_ptr<UsbDevice> udev);
    void remove_udev(std::shared_ptr<UsbDevice> udev);
    void print_all_udevs_info();
    void print_new_udev_info(const std::shared_ptr<UsbDevice> &udev);
    int get_udev_size();
    
private:
    UsbManager();
    ~UsbManager();


private:
    std::shared_ptr<UsbMonitor> monitor_ptr_;
    std::vector<std::unique_ptr<UsbObserver>> observers_;
    static std::vector<std::shared_ptr<UsbDevice>> udevs_;
    UsbCommonTyps::Callbacks callbacks_;
};

#endif