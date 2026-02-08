#include "usb_device_manager.hpp"

UsbCommonTyps::UsbDeviceMapPtr UsbDeviceManager::devs_ptr_ = std::make_shared<UsbCommonTyps::UsbDeviceMap>();
UsbCommonTyps::DeviceNum UsbDeviceManager::num_of_various_dev_;

UsbDeviceManager::UsbDeviceManager() : mon_ptr_(std::make_shared<UsbDeviceMonitor>())
{
    set_callback();
}

UsbDeviceManager::~UsbDeviceManager()
{
    stop();
}

UsbDeviceManager& UsbDeviceManager::get_instance()  {
    static UsbDeviceManager usb_manager;
    return usb_manager;
}

int UsbDeviceManager::start() {
    enumerate_usb_device();
    print_all_usb_device_info();
    print_all_usb_interface_info();
    monitor_start();
    return 0;
}

int UsbDeviceManager::stop() {
    monitor_stop();
    return 0;
}

void UsbDeviceManager::set_callback () {
    callbacks_ = {
        [this](std::shared_ptr<UsbDevice> udev) {
            add_usb_device(udev); 
        },

        [this](std::shared_ptr<UsbDevice> udev) {
            remove_usb_device(udev);
        },

        [this](const std::shared_ptr<UsbDevice> &udev) {
            print_new_usb_device_info(udev);
        },

        [this]() {
            print_all_usb_device_info();
        },

        [this](const std::shared_ptr<UsbInterface> &udev) {
            print_new_interface_info(udev);
        },

        [this]() {
            print_all_usb_interface_info();
        },

        [this]() {
            return get_usb_device_total_num();
        },

        [this]() {
            return get_usb_interface_number();
        },
        
        [this]() {
            return get_devs_ptr();
        },

        [this]() {
            return get_num_of_various_dev();
        }
    };
    mon_ptr_->set_callback(callbacks_);
}

int UsbDeviceManager::monitor_start() {
    return mon_ptr_->start();
}

int UsbDeviceManager::monitor_stop() {
    return mon_ptr_->stop();
}

bool UsbDeviceManager::enumerate_usb_device() {

    UsbCommonTyps::UdevDelPtr udev(udev_new());
    if (!udev) {
        LOG_ERROR("udev_new faild!");
        return false;
    }

    UsbCommonTyps::UdevEnumerateDelPtr en(udev_enumerate_new(udev.get()));
    if (!en) {
        LOG_ERROR("udev_enumerate_new faild!");
        return false;
    }

    udev_enumerate_add_match_subsystem(en.get(), "usb");
    udev_enumerate_scan_devices(en.get());

    struct udev_list_entry *entry;
    struct udev_list_entry *list = udev_enumerate_get_list_entry(en.get());
    
    udev_list_entry_foreach(entry, list) {
        const char *path = udev_list_entry_get_name(entry);
        UsbCommonTyps::UdevDeviceDelPtr dev(udev_device_new_from_syspath(udev.get(), path));
        if (!dev) {
            LOG_WARN("udev_device_new_from_syspath faild, path={}", path);
            continue;
        }

        const char *devtype = udev_device_get_devtype(dev.get());
        if (devtype && strcmp(devtype, "usb_device") == 0) {
            auto device_ptr = mon_ptr_->parse_usb_device(devs_ptr_, dev.get());
            if (device_ptr) add_usb_device(device_ptr);
        } else if (devtype && strcmp(devtype, "usb_interface") == 0) {
            auto device_ptr = mon_ptr_->parse_usb_interface(devs_ptr_, dev.get());
            if (device_ptr) add_usb_device(device_ptr);
        }
    }

    return true;
}

void UsbDeviceManager::print_all_usb_device_info() {

    LOG_WARN("USB设备数量:{}", get_usb_device_total_num());
    for (auto it = devs_ptr_->begin(); it != devs_ptr_->end(); it++) {
        auto usb_dev_info = it->second->get_usb_device_info();
        LOG_INFO("\n\
                SYSPATH={}\n\
                SYSNAME={}\n\
                SYSNUM={}\n\
                SUBSYSTEM={}\n\
                DEVPATH={}\n\
                DEVNODE={}\n\
                DEVTYPE={}\n\
                ACTION={}\n\
                DRIVER={}\n\
                PRODUCT={}\n\
                VENDOR_ID={}\n\
                PRODUCT_ID={}\n\
                MANUFACTURER={}\n\
                SERIAL={}\n\
                BUSNUM={}\n\
                DEVNUM={}\n",\
                usb_dev_info->base_info.syspath,\
                usb_dev_info->base_info.sysname,\
                usb_dev_info->sysnum,\
                usb_dev_info->base_info.subsystem,\
                usb_dev_info->base_info.devpath,\
                usb_dev_info->devnode,\
                usb_dev_info->base_info.devtype,\
                usb_dev_info->action,\
                usb_dev_info->base_info.driver,\
                usb_dev_info->product,\
                usb_dev_info->vendor_id,\
                usb_dev_info->product_id,\
                usb_dev_info->manufacturer,\
                usb_dev_info->serial,\
                usb_dev_info->busnum,\
                usb_dev_info->devnum
        );
    }
}

void UsbDeviceManager::print_all_usb_interface_info() {
    LOG_WARN("USB设备接口数量:{}", get_usb_interface_number());
    for (auto it = devs_ptr_->begin(); it != devs_ptr_->end(); it++) {
        auto usb_interfaces_info = it->second->get_usb_interfaces_info();
        for (auto &info : usb_interfaces_info) {
            LOG_INFO("\n\
                    SYSPATH={}\n\
                    SYSNAME={}\n\
                    SUBSYSTEM={}\n\
                    DEVPATH={}\n\
                    DEVTYPE={}\n\
                    DRIVER={}\n\
                    AUTHORIZED={}\n\
                    bAlternateSetting={}\n\
                    bInterfaceClass={}\n\
                    bInterfaceNumber={}\n\
                    bInterfaceProtocol={}\n\
                    bInterfaceSubClass={}\n\
                    bNumEndpoints={}\n\
                    modalias={}\n\
                    supports_autosuspend={}\n\
                    uevent={}\n",\
                    info->base_info.syspath,\
                    info->base_info.sysname,\
                    info->base_info.subsystem,\
                    info->base_info.devpath,\
                    info->base_info.devtype,\
                    info->base_info.driver,\
                    info->authorized,\
                    info->bAlternateSetting,\
                    info->bInterfaceClass,\
                    info->bInterfaceNumber,\
                    info->bInterfaceProtocol,\
                    info->bInterfaceSubClass,\
                    info->bNumEndpoints,\
                    info->modalias,\
                    info->supports_autosuspend,\
                    // info->uevent
                    ""
            );
        }
    }
}

void UsbDeviceManager::print_new_usb_device_info(const std::shared_ptr<UsbDevice> &usb_dev) {

    if (!usb_dev) {
        LOG_ERROR("usb_dev is nullptr!");
        return;
    }

    auto usb_dev_info = usb_dev->get_usb_device_info();
    LOG_INFO("\n\
                SYSPATH={}\n\
                SYSNAME={}\n\
                SYSNUM={}\n\
                SUBSYSTEM={}\n\
                DEVPATH={}\n\
                DEVNODE={}\n\
                DEVTYPE={}\n\
                ACTION={}\n\
                DRIVER={}\n\
                PRODUCT={}\n\
                VENDOR_ID={}\n\
                PRODUCT_ID={}\n\
                MANUFACTURER={}\n\
                SERIAL={}\n\
                BUSNUM={}\n\
                DEVNUM={}\n",\
                usb_dev_info->base_info.syspath,\
                usb_dev_info->base_info.sysname,\
                usb_dev_info->sysnum,\
                usb_dev_info->base_info.subsystem,\
                usb_dev_info->base_info.devpath,\
                usb_dev_info->devnode,\
                usb_dev_info->base_info.devtype,\
                usb_dev_info->action,\
                usb_dev_info->base_info.driver,\
                usb_dev_info->product,\
                usb_dev_info->vendor_id,\
                usb_dev_info->product_id,\
                usb_dev_info->manufacturer,\
                usb_dev_info->serial,\
                usb_dev_info->busnum,\
                usb_dev_info->devnum
        );
}

void UsbDeviceManager::print_new_interface_info(const std::shared_ptr<UsbInterface> &usb_dev) {

    if (!usb_dev) {
        LOG_ERROR("usb_dev is nullptr!");
        return;
    }

    auto info = usb_dev->get_usb_interface_info();
    LOG_INFO("\n\
                SYSPATH={}\n\
                SYSNAME={}\n\
                SUBSYSTEM={}\n\
                DEVPATH={}\n\
                DEVTYPE={}\n\
                DRIVER={}\n\
                AUTHORIZED={}\n\
                bAlternateSetting={}\n\
                bInterfaceClass={}\n\
                bInterfaceNumber={}\n\
                bInterfaceProtocol={}\n\
                bInterfaceSubClass={}\n\
                bNumEndpoints={}\n\
                modalias={}\n\
                supports_autosuspend={}\n\
                uevent={}\n",\
                info->base_info.syspath,\
                info->base_info.sysname,\
                info->base_info.subsystem,\
                info->base_info.devpath,\
                info->base_info.devtype,\
                info->base_info.driver,\
                info->authorized,\
                info->bAlternateSetting,\
                info->bInterfaceClass,\
                info->bInterfaceNumber,\
                info->bInterfaceProtocol,\
                info->bInterfaceSubClass,\
                info->bNumEndpoints,\
                info->modalias,\
                info->supports_autosuspend,\
                // info->uevent
                ""
    );
}

void UsbDeviceManager::add_usb_device(std::shared_ptr<UsbDevice> usb_dev) {
    std::string syspath = usb_dev->get_syspath();
    if (!devs_ptr_->count(syspath)) devs_ptr_->emplace(syspath, std::move(usb_dev));
}

void UsbDeviceManager::remove_usb_device(std::shared_ptr<UsbDevice> usb_dev) {
    devs_ptr_->erase(usb_dev->get_syspath());
}

int UsbDeviceManager::get_usb_device_total_num() {
    return devs_ptr_->size();
}

UsbCommonTyps::DeviceNum UsbDeviceManager::get_num_of_various_dev() {
    return num_of_various_dev_;
}

int UsbDeviceManager::get_usb_interface_number() {

    int count = 0;
    for (auto it = devs_ptr_->begin(); it != devs_ptr_->end(); it++) {
        if (!it->second) continue;
        count += it->second->get_usb_interface_number();
    }

    return count;
}

UsbCommonTyps::UsbDeviceMapPtr UsbDeviceManager::get_devs_ptr() {
    return devs_ptr_;
}