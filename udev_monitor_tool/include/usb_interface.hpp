#ifndef USB_INTERFACE_HPP
#define USB_INTERFACE_HPP
#include "common.hpp"
#include "udev_common_types.hpp"

class UsbInterface {

public:
    UsbInterface();
    ~UsbInterface();

public:
    enum class InterfaceState {
        INIT,
        UNINIT
    };

public:
    void set_syspath(std::string syspath);
    void set_sysname(std::string sysname);
    void set_subsystem(std::string subsystem);
    void set_devpath(std::string devpath);
    void set_devtype(std::string devtype);
    void set_driver(std::string driver);
    void set_authorized(std::string authorized);
    void set_bAlternateSetting(std::string bAlternateSetting);
    void set_bInterfaceClass(std::string bInterfaceClass);
    void set_bInterfaceNumber(std::string bInterfaceNumber);
    void set_bInterfaceProtocol(std::string bInterfaceProtocol);
    void set_bInterfaceSubClass(std::string bInterfaceSubClass);
    void set_bNumEndpoints(std::string bNumEndpoints);
    void set_modalias(std::string modalias);
    void set_supports_autosuspend(std::string supports_autosuspend);
    void set_uevent(std::string uevent);
    void set_usb_interface_state(InterfaceState state);

public:
    std::string get_syspath();
    std::string get_sysname();
    std::string get_subsystem();
    std::string get_devpath();
    std::string get_devtype();
    std::string get_driver();
    std::string get_authorized();
    std::string get_bAlternateSetting();
    std::string get_bInterfaceClass();
    std::string get_bInterfaceNumber();
    std::string get_bInterfaceProtocol();
    std::string get_bInterfaceSubClass();
    std::string get_bNumEndpoints();
    std::string get_modalias();
    std::string get_supports_autosuspend();
    std::string get_uevent();
    std::shared_ptr<UsbCommonTyps::UsbInterfaceInfo> get_usb_interface_info();
    InterfaceState get_usb_interface_state();


private:
    int init();

public:
    
private:
    InterfaceState state_;
    std::shared_ptr<UsbCommonTyps::UsbInterfaceInfo> usb_interface_info_ptr_;
};

#endif