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
    udev_info_ptr_->syspath = syspath;
}

void UsbDevice::set_sysname(std::string sysname) {
    udev_info_ptr_->sysname = sysname;
}

void UsbDevice::set_sysnum(std::string sysnum) {
    udev_info_ptr_->sysnum = sysnum;
}

void UsbDevice::set_devpath(std::string devpath) {
    udev_info_ptr_->devpath = devpath;
}

void UsbDevice::set_devnode(std::string devnode) {
    udev_info_ptr_->devnode = devnode;
}

void UsbDevice::set_action(std::string action) {
    udev_info_ptr_->action = action;
}

void UsbDevice::set_subsystem(std::string subsystem) {
    udev_info_ptr_->subsystem = subsystem;
}

void UsbDevice::set_driver(std::string driver) {
    udev_info_ptr_->driver = driver;
}

void UsbDevice::set_devtype(std::string devtype) {
    udev_info_ptr_->devtype = devtype;
}

void UsbDevice::set_vendor_id(std::string vendor_id) {
    udev_info_ptr_->vendor_id = vendor_id;
}

void UsbDevice::set_product_id(std::string product_id) {
    udev_info_ptr_->product_id = product_id;
}

void UsbDevice::set_manufacturer(std::string manufacturer) {
    udev_info_ptr_->manufacturer = manufacturer;
}

void UsbDevice::set_product(std::string product) {
    udev_info_ptr_->product = product;
}

void UsbDevice::set_serial(std::string serial) {
    udev_info_ptr_->serial = serial;
}

void UsbDevice::set_busnum(int busnum) {
    udev_info_ptr_->busnum = busnum;
}

void UsbDevice::set_devnum(int devnum) {
    udev_info_ptr_->devnum = devnum;
}

std::string UsbDevice::get_syspath() {
    return udev_info_ptr_->syspath;
}

std::string UsbDevice::get_sysname() {
    return udev_info_ptr_->sysname;
}

std::string UsbDevice::get_sysnum() {
    return udev_info_ptr_->sysnum;
}

std::string UsbDevice::get_devpath() {
    return udev_info_ptr_->devpath;
}

std::string UsbDevice::get_devnode() {
    return udev_info_ptr_->devnode;
}

std::string UsbDevice::get_action() {
    return udev_info_ptr_->action;
}

std::string UsbDevice::get_subsystem() {
    return udev_info_ptr_->subsystem;
}

std::string UsbDevice::get_driver() {
    return udev_info_ptr_->driver;
}

std::string UsbDevice::get_devtype() {
    return udev_info_ptr_->devtype;
}

std::string UsbDevice::get_vendor_id() {
    return udev_info_ptr_->vendor_id;
}

std::string UsbDevice::get_product_id() {
    return udev_info_ptr_->product_id;
}

std::string UsbDevice::get_manufacturer() {
    return udev_info_ptr_->manufacturer;
}

std::string UsbDevice::get_product() {
    return udev_info_ptr_->product;
}

std::string UsbDevice::get_serial() {
    return udev_info_ptr_->serial;
}

int UsbDevice::get_busnum() {
    return udev_info_ptr_->busnum;
}

int UsbDevice::get_devnum() {
    return udev_info_ptr_->devnum;
}

std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> UsbDevice::get_all_udevs_info() {
    return udev_info_ptr_;
}