#include "usb_device.hpp"

UsbDevice::UsbDevice() : state_(UsbDeviceState::UNINIT),
                         usb_dev_info_ptr_(std::make_shared<UsbCommonTyps::UsbDeviceInfo>()),
                         latest_interface_ptr_(std::make_shared<UsbInterface>())
{

}

UsbDevice::~UsbDevice()
{

}

void UsbDevice::set_usb_dev_state(UsbDeviceState state) {
    state_ = state;
}

void UsbDevice::set_syspath(std::string syspath) {
    usb_dev_info_ptr_->base_info.syspath = syspath;
}

void UsbDevice::set_sysname(std::string sysname) {
    usb_dev_info_ptr_->base_info.sysname = sysname;
}

void UsbDevice::set_sysnum(std::string sysnum) {
    usb_dev_info_ptr_->sysnum = sysnum;
}

void UsbDevice::set_devpath(std::string devpath) {
    usb_dev_info_ptr_->base_info.devpath = devpath;
}

void UsbDevice::set_devname(UsbCommonTyps::InterfaceDescriptor &describe, std::shared_ptr<UsbCommonTyps::DeviceNum> num) {

    std::string devname;
    if (describe.interface_class == "02") {
        if (describe.interface_subclass == "02") {
            devname = "虚拟串口" + std::to_string(num->num_type_virtualcomport++ + 1);
        } else if (describe.interface_subclass == "06") {
            devname = "网卡" + std::to_string(num->num_type_netcard++ + 1);
        }
    } else if (describe.interface_class == "03" && describe.interface_subclass == "01") {
        if (describe.interface_protocol == "01") {
            devname = "键盘" + std::to_string(num->num_type_keyboard++ + 1);
        } else if (describe.interface_protocol == "02") {
            devname = "鼠标" + std::to_string(num->num_type_mouse++ + 1);
        }
    } else if (describe.interface_class == "07") {
        devname = "打印机" + std::to_string(num->num_type_printer++ + 1);
    } else if (describe.interface_class == "08") {
        if (describe.interface_subclass == "02") {
            devname = "光驱" + std::to_string(num->num_type_optical++ + 1);
        } else if (describe.interface_subclass == "04") {
            devname = "软驱" + std::to_string(num->num_type_floppy++ + 1);
        } else if (describe.interface_subclass == "06") {
            devname = "硬盘" + std::to_string(num->num_type_disk++ + 1);
        }
    } else if (describe.interface_class == "0ff") {
        devname = "自定义设备" + std::to_string(num->num_type_custom++ + 1);
    } else {
        LOG_ERROR("未知的USB设备类型!");
    }

    num->num_total++;
    usb_dev_info_ptr_->base_info.devname = devname;
}

void UsbDevice::set_devnode(std::string devnode) {
    usb_dev_info_ptr_->devnode = devnode;
}

void UsbDevice::set_action(std::string action) {
    usb_dev_info_ptr_->action = action;
}

void UsbDevice::set_subsystem(std::string subsystem) {
    usb_dev_info_ptr_->base_info.subsystem = subsystem;
}

void UsbDevice::set_driver(std::string driver) {
    usb_dev_info_ptr_->base_info.driver = driver;
}

void UsbDevice::set_devtype(std::string devtype) {
    usb_dev_info_ptr_->base_info.devtype = devtype;
}

void UsbDevice::set_vendor_id(std::string vendor_id) {
    usb_dev_info_ptr_->vendor_id = vendor_id;
}

void UsbDevice::set_product_id(std::string product_id) {
    usb_dev_info_ptr_->product_id = product_id;
}

void UsbDevice::set_manufacturer(std::string manufacturer) {
    usb_dev_info_ptr_->manufacturer = manufacturer;
}

void UsbDevice::set_product(std::string product) {
    usb_dev_info_ptr_->product = product;
}

void UsbDevice::set_serial(std::string serial) {
    usb_dev_info_ptr_->serial = serial;
}

void UsbDevice::set_busnum(int busnum) {
    usb_dev_info_ptr_->busnum = busnum;
}

void UsbDevice::set_devnum(int devnum) {
    usb_dev_info_ptr_->devnum = devnum;
}

void UsbDevice::set_removable(std::string removable) {
    usb_dev_info_ptr_->removable = removable;
}

void UsbDevice::set_maxchild(std::string maxchild) {
    usb_dev_info_ptr_->maxchild = maxchild;
}

void UsbDevice::set_latest_interface(std::shared_ptr<UsbInterface> &interface) {
    latest_interface_ptr_ = interface;
}

UsbDevice::UsbDeviceState UsbDevice::get_usb_dev_state() {
    return state_;
}

std::string UsbDevice::get_syspath() {
    return usb_dev_info_ptr_->base_info.syspath;
}

std::string UsbDevice::get_sysname() {
    return usb_dev_info_ptr_->base_info.sysname;
}

std::string UsbDevice::get_sysnum() {
    return usb_dev_info_ptr_->sysnum;
}

std::string UsbDevice::get_devpath() {
    return usb_dev_info_ptr_->base_info.devpath;
}

std::string UsbDevice::get_devnode() {
    return usb_dev_info_ptr_->devnode;
}

std::string UsbDevice::get_action() {
    return usb_dev_info_ptr_->action;
}

std::string UsbDevice::get_subsystem() {
    return usb_dev_info_ptr_->base_info.subsystem;
}

std::string UsbDevice::get_driver() {
    return usb_dev_info_ptr_->base_info.driver;
}

std::string UsbDevice::get_devtype() {
    return usb_dev_info_ptr_->base_info.devtype;
}

std::string UsbDevice::get_vendor_id() {
    return usb_dev_info_ptr_->vendor_id;
}

std::string UsbDevice::get_product_id() {
    return usb_dev_info_ptr_->product_id;
}

std::string UsbDevice::get_manufacturer() {
    return usb_dev_info_ptr_->manufacturer;
}

std::string UsbDevice::get_product() {
    return usb_dev_info_ptr_->product;
}

std::string UsbDevice::get_serial() {
    return usb_dev_info_ptr_->serial;
}

int UsbDevice::get_busnum() {
    return usb_dev_info_ptr_->busnum;
}

int UsbDevice::get_devnum() {
    return usb_dev_info_ptr_->devnum;
}

std::string UsbDevice::get_removable() {
    return usb_dev_info_ptr_->removable;
}

std::string UsbDevice::get_maxchild() {
    return usb_dev_info_ptr_->maxchild;
}

std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> UsbDevice::get_usb_device_info() {
    return usb_dev_info_ptr_;
}

std::vector<std::shared_ptr<UsbCommonTyps::UsbInterfaceInfo>> UsbDevice::get_usb_interfaces_info() {

    if (interfaces_.empty()) return {};

    std::vector<std::shared_ptr<UsbCommonTyps::UsbInterfaceInfo>> interfaces;
    for (auto &it : interfaces_) {
        interfaces.push_back(it.second->get_usb_interface_info());
    }
    
    return std::move(interfaces);
}

int UsbDevice::get_usb_interface_number() {
    return interfaces_.size();
}

std::shared_ptr<UsbInterface> UsbDevice::get_latest_interface() {
    return latest_interface_ptr_;
}