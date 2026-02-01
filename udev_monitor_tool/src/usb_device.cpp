#include "usb_device.hpp"

UsbDevice::UsbDevice() : state_(UsbDeviceState::UNINIT) {
    init();
}

UsbDevice::~UsbDevice() {

}

int UsbDevice::init() {
    usb_dev_info_ptr_ = std::make_shared<UsbCommonTyps::UsbDeviceInfo>();
    return 0;
}

void UsbDevice::set_usb_dev_state(UsbDeviceState state) {
    state_ = state;
}

void UsbDevice::set_syspath(std::string syspath) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->base_info.syspath = syspath;
}

void UsbDevice::set_sysname(std::string sysname) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->base_info.sysname = sysname;
}

void UsbDevice::set_sysnum(std::string sysnum) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->sysnum = sysnum;
}

void UsbDevice::set_devpath(std::string devpath) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->base_info.devpath = devpath;
}

void UsbDevice::set_devnode(std::string devnode) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->devnode = devnode;
}

void UsbDevice::set_action(std::string action) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->action = action;
}

void UsbDevice::set_subsystem(std::string subsystem) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->base_info.subsystem = subsystem;
}

void UsbDevice::set_driver(std::string driver) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->base_info.driver = driver;
}

void UsbDevice::set_devtype(std::string devtype) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->base_info.devtype = devtype;
}

void UsbDevice::set_vendor_id(std::string vendor_id) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->vendor_id = vendor_id;
}

void UsbDevice::set_product_id(std::string product_id) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->product_id = product_id;
}

void UsbDevice::set_manufacturer(std::string manufacturer) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->manufacturer = manufacturer;
}

void UsbDevice::set_product(std::string product) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->product = product;
}

void UsbDevice::set_serial(std::string serial) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->serial = serial;
}

void UsbDevice::set_busnum(int busnum) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->busnum = busnum;
}

void UsbDevice::set_devnum(int devnum) {
    if (usb_dev_info_ptr_) usb_dev_info_ptr_->devnum = devnum;
}

UsbDevice::UsbDeviceState UsbDevice::get_usb_dev_state() {
    return state_;
}

std::string UsbDevice::get_syspath() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->base_info.syspath : "";
}

std::string UsbDevice::get_sysname() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->base_info.sysname : "";
}

std::string UsbDevice::get_sysnum() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->sysnum : "";
}

std::string UsbDevice::get_devpath() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->base_info.devpath : "";
}

std::string UsbDevice::get_devnode() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->devnode : "";
}

std::string UsbDevice::get_action() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->action : "";
}

std::string UsbDevice::get_subsystem() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->base_info.subsystem : "";
}

std::string UsbDevice::get_driver() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->base_info.driver : "";
}

std::string UsbDevice::get_devtype() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->base_info.devtype : "";
}

std::string UsbDevice::get_vendor_id() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->vendor_id : "";
}

std::string UsbDevice::get_product_id() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->product_id : "";
}

std::string UsbDevice::get_manufacturer() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->manufacturer : "";
}

std::string UsbDevice::get_product() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->product : "";
}

std::string UsbDevice::get_serial() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->serial : "";
}

int UsbDevice::get_busnum() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->busnum : -1;
}

int UsbDevice::get_devnum() {
    return usb_dev_info_ptr_ ? usb_dev_info_ptr_->devnum : -1;
}

std::shared_ptr<UsbCommonTyps::UsbDeviceInfo> UsbDevice::get_usb_device_info() {
    return usb_dev_info_ptr_;
}

std::shared_ptr<UsbCommonTyps::UsbInterfaceInfo> UsbDevice::get_usb_interface_info() {
    return interface_ptr_ ? interface_ptr_->get_usb_interface_info() : nullptr;
}