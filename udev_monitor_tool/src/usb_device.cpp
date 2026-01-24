#include "usb_device.hpp"

UsbDevice::UsbDevice() {
    init();
}

UsbDevice::~UsbDevice() {

}

int UsbDevice::init() {
    udev_info_ptr_ = std::make_shared<UsbCommonTyps::UsbDeviceInfo>();
    return 0;
}

void UsbDevice::set_syspath(std::string syspath) {
    udev_info_ptr_->syspath_ = syspath;
}

void UsbDevice::set_devnode(std::string devnode) {
    udev_info_ptr_->devnode_ = devnode;
}

void UsbDevice::set_action(std::string action) {
    udev_info_ptr_->action_ = action;
}

void UsbDevice::set_subsystem(std::string subsystem) {
    udev_info_ptr_->subsystem_ = subsystem;
}

void UsbDevice::set_devtype(std::string devtype) {
    udev_info_ptr_->devtype_ = devtype;
}

void UsbDevice::set_vendor_id(std::string vendor_id) {
    udev_info_ptr_->vendor_id_ = vendor_id;
}

void UsbDevice::set_product_id(std::string product_id) {
    udev_info_ptr_->product_id_ = product_id;
}

void UsbDevice::set_manufacturer(std::string manufacturer) {
    udev_info_ptr_->manufacturer_ = manufacturer;
}

void UsbDevice::set_product(std::string product) {
    udev_info_ptr_->product_ = product;
}

void UsbDevice::set_serial(std::string serial) {
    udev_info_ptr_->serial_ = serial;
}

void UsbDevice::set_busnum(int busnum) {
    udev_info_ptr_->busnum_ = busnum;
}

void UsbDevice::set_devnum(int devnum) {
    udev_info_ptr_->devnum_ = devnum;
}

std::string UsbDevice::get_syspath() {
    return udev_info_ptr_->syspath_;
}

std::string UsbDevice::get_devnode() {
    return udev_info_ptr_->devnode_;
}

std::string UsbDevice::get_action() {
    return udev_info_ptr_->action_;
}

std::string UsbDevice::get_subsystem() {
    return udev_info_ptr_->subsystem_;
}

std::string UsbDevice::get_devtype() {
    return udev_info_ptr_->devtype_;
}

std::string UsbDevice::get_vendor_id() {
    return udev_info_ptr_->vendor_id_;
}

std::string UsbDevice::get_product_id() {
    return udev_info_ptr_->product_id_;
}

std::string UsbDevice::get_manufacturer() {
    return udev_info_ptr_->manufacturer_;
}

std::string UsbDevice::get_product() {
    return udev_info_ptr_->product_;
}

std::string UsbDevice::get_serial() {
    return udev_info_ptr_->serial_;
}

int UsbDevice::get_busnum() {
    return udev_info_ptr_->busnum_;
}

int UsbDevice::get_devnum() {
    return udev_info_ptr_->devnum_;
}

std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> UsbDevice::get_all_udevs_info() {
    return udev_info_ptr_;
}