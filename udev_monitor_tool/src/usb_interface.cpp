#include "usb_interface.hpp"

UsbInterface::UsbInterface() : state_(InterfaceState::UNINIT) {
    init();
}

UsbInterface::~UsbInterface() {

}

int UsbInterface::init() {
    usb_interface_info_ptr_ = std::make_shared<UsbCommonTyps::UsbInterfaceInfo>();
    return 0;
}

void UsbInterface::set_syspath(std::string syspath) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->base_info.syspath = syspath;
}

void UsbInterface::set_sysname(std::string sysname) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->base_info.sysname = sysname;
}

void UsbInterface::set_subsystem(std::string subsystem) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->base_info.subsystem = subsystem;
}

void UsbInterface::set_devpath(std::string devpath) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->base_info.devpath = devpath;
}

void UsbInterface::set_devtype(std::string devtype) {
   if (usb_interface_info_ptr_) usb_interface_info_ptr_->base_info.devtype= devtype;
}

void UsbInterface::set_driver(std::string driver) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->base_info.driver = driver;
}

void UsbInterface::set_authorized(std::string authorized) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->authorized = authorized;
}

void UsbInterface::set_bAlternateSetting(std::string bAlternateSetting) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->bAlternateSetting = bAlternateSetting;
}

void UsbInterface::set_bInterfaceClass(std::string bInterfaceClass) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->bInterfaceClass = bInterfaceClass;
}

void UsbInterface::set_bInterfaceNumber(std::string bInterfaceNumber) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->bInterfaceNumber = bInterfaceNumber;
}

void UsbInterface::set_bInterfaceProtocol(std::string bInterfaceProtocol) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->bInterfaceProtocol = bInterfaceProtocol;
}

void UsbInterface::set_bInterfaceSubClass(std::string bInterfaceSubClass) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->bInterfaceSubClass = bInterfaceSubClass;
}

void UsbInterface::set_bNumEndpoints(std::string bNumEndpoints) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->bNumEndpoints = bNumEndpoints;
}

void UsbInterface::set_modalias(std::string modalias) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->modalias = modalias;
}

void UsbInterface::set_supports_autosuspend(std::string supports_autosuspend) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->supports_autosuspend = supports_autosuspend;
}

void UsbInterface::set_uevent(std::string uevent) {
    if (usb_interface_info_ptr_) usb_interface_info_ptr_->uevent = uevent;
}

void UsbInterface::set_usb_interface_state(InterfaceState state) {
    state_ = state;
}

std::string UsbInterface::get_syspath() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->base_info.syspath : "";
}

std::string UsbInterface::get_sysname() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->base_info.sysname : "";
}

std::string UsbInterface::get_subsystem() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->base_info.subsystem : "";
}

std::string UsbInterface::get_devpath() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->base_info.devpath : "";
}

std::string UsbInterface::get_devtype() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->base_info.devtype : "";
}

std::string UsbInterface::get_driver() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->base_info.driver : "";
}

std::string UsbInterface::get_authorized() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->authorized : "";
}

std::string UsbInterface::get_bAlternateSetting() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->bAlternateSetting : "";
}

std::string UsbInterface::get_bInterfaceClass() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->bInterfaceClass : "";
}

std::string UsbInterface::get_bInterfaceNumber() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->bInterfaceNumber : "";
}

std::string UsbInterface::get_bInterfaceProtocol() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->bInterfaceProtocol : "";
}

std::string UsbInterface::get_bInterfaceSubClass() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->bInterfaceSubClass : "";
}

std::string UsbInterface::get_bNumEndpoints() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->bNumEndpoints : "";
}

std::string UsbInterface::get_modalias() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->modalias : "";
}

std::string UsbInterface::get_supports_autosuspend() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->supports_autosuspend : "";
}

std::string UsbInterface::get_uevent() {
    return usb_interface_info_ptr_ ? usb_interface_info_ptr_->uevent : "";
}

std::shared_ptr<UsbCommonTyps::UsbInterfaceInfo> UsbInterface::get_usb_interface_info() {
    return usb_interface_info_ptr_;
}

UsbInterface::InterfaceState UsbInterface::get_usb_interface_state() {
    return state_;
}