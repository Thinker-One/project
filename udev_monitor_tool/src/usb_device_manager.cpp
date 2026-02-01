#include "usb_device_manager.hpp"

UsbCommonTyps::UsbDeviceMapPtr UsbDeviceManager::devs_ptr_ = std::make_shared<UsbCommonTyps::UsbDeviceMap>();

UsbDeviceManager::UsbDeviceManager() {
    init();
}

UsbDeviceManager::~UsbDeviceManager() {
    monitor_stop();
}

UsbDeviceManager& UsbDeviceManager::get_instance()  {
    static UsbDeviceManager usb_manager;
    return usb_manager;
}

void UsbDeviceManager::init() {
    enumerate_usb_device();
    mon_ptr_ = std::make_shared<UsbDeviceMonitor>();
    set_callback();
    print_all_usb_device_info();
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

        [this]() {
            return get_usb_device_number();
        },
        
        [this]() {
            return get_devs_ptr_();
        }
    };
    if (mon_ptr_) mon_ptr_->set_callback(callbacks_);
}

int UsbDeviceManager::monitor_start() {
    return mon_ptr_ ? mon_ptr_->start() : 0;
}

int UsbDeviceManager::monitor_stop() {
    return mon_ptr_ ? mon_ptr_->stop() : 0;
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

    LOG_INFO("CUR UDEV COUNT:{}.", get_usb_device_number());
    for (auto it = devs_ptr_->begin(); it != devs_ptr_->end(); it++) {
        auto usb_dev_info = it->second->get_usb_device_info();
        if (!usb_dev_info) {
            LOG_ERROR("usb_dev_info is nullptr!");
            return;
        }
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

void UsbDeviceManager::print_new_usb_device_info(const std::shared_ptr<UsbDevice> &usb_dev) {

    if (!usb_dev) {
        LOG_ERROR("usb_dev is nullptr!");
        return;
    }

    auto usb_dev_info = usb_dev->get_usb_device_info();
    if (!usb_dev_info) {
        LOG_ERROR("usb_dev_info is nullptr!");
        return;
    }

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

void UsbDeviceManager::add_usb_device(std::shared_ptr<UsbDevice> usb_dev) {
    
    if (!usb_dev) {
        LOG_ERROR("usb_dev is nullptr!");
        return;
    }

    if (!devs_ptr_) {
        LOG_ERROR("devs_ptr_ is nullptr!");
        return;
    }
    
    std::string syspath = usb_dev->get_syspath();
    try {
        if (devs_ptr_->count(syspath) && devs_ptr_->at(syspath)->get_usb_dev_state() == UsbDevice::UsbDeviceState::INIT) return;
    } catch (const std::exception &e) {
        LOG_ERROR("error={}", e.what());
    }
    devs_ptr_->emplace(syspath, std::move(usb_dev));
}

void UsbDeviceManager::remove_usb_device(std::shared_ptr<UsbDevice> usb_dev_b) {

    if (!usb_dev_b) {
        LOG_ERROR("usb_dev_b is nullptr!");
        return;
    }

    if (!devs_ptr_) {
        LOG_ERROR("devs_ptr_ is nullptr!");
        return;
    }

    devs_ptr_->erase(usb_dev_b->get_syspath());
}

int UsbDeviceManager::get_usb_device_number() {
    return devs_ptr_->size();
}

UsbCommonTyps::UsbDeviceMapPtr UsbDeviceManager::get_devs_ptr_() {
    return devs_ptr_;
}