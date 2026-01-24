#include "usb_manager.hpp"

std::vector<std::shared_ptr<UsbDevice>> UsbManager::udevs_;

UsbManager::UsbManager() {
    init();
}

UsbManager::~UsbManager() {
    monitor_stop();
}

UsbManager& UsbManager::get_instance()  {
    static UsbManager usb_manager;
    return usb_manager;
}

void UsbManager::init() {
    monitor_ptr_ = std::make_shared<UsbMonitor>();
    set_callback();
    enumerate_udevs();
    print_all_udevs_info();
}

void UsbManager::set_callback () {
    callbacks_ = {
        [this](std::shared_ptr<UsbDevice> udev) { add_udev(udev); },
        [this](std::shared_ptr<UsbDevice> udev) { remove_udev(udev); },
        [this](const std::shared_ptr<UsbDevice> &udev) { print_new_udev_info(udev); },
        [this]() { print_all_udevs_info(); },
        [this]() { return get_udev_size(); }
    };
    monitor_ptr_->set_callback(callbacks_);
}

int UsbManager::monitor_start() {
    return monitor_ptr_ ? monitor_ptr_->start() : 0;
}

int UsbManager::monitor_stop() {
    return monitor_ptr_ ? monitor_ptr_->stop() : 0;
}

bool UsbManager::enumerate_udevs() {

    struct udev *udev = udev_new();
    if (!udev) return false;

    struct udev_enumerate *en = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(en, "usb");
    udev_enumerate_scan_devices(en);

    struct udev_list_entry *entry;
    struct udev_list_entry *list =
        udev_enumerate_get_list_entry(en);

    udev_list_entry_foreach(entry, list) {
        const char *path = udev_list_entry_get_name(entry);
        struct udev_device *dev = udev_device_new_from_syspath(udev, path);
        if (!dev) continue;

        const char *devtype = udev_device_get_devtype(dev);
        if (devtype && strcmp(devtype, "usb_device") == 0) {
            add_udev(monitor_ptr_->parse_udev(dev));
        }
        udev_device_unref(dev);
    }

    udev_enumerate_unref(en);
    udev_unref(udev);
    return true;
}

void UsbManager::print_all_udevs_info() {
    LOG_INFO("CUR UDEV COUNT:{}.", get_udev_size());
    for (auto &it : udevs_) {
        auto udev_info = it->get_all_udevs_info();
        LOG_INFO("\n\
                SYSPATH={}\n\
                DEVNODE={}\n\
                ACTION={}\n\
                SUBSYSTEM={}\n\
                DEVTYPE={}\n\
                VENDOR_ID={}\n\
                PRODUCT_ID={}\n\
                MANUFACTURER={}\n\
                PRODUCT={}\n\
                SERIAL_={}\n\
                BUSNUM={}\n\
                DEVNUM={}\n", \
                udev_info->syspath_,\
                udev_info->devnode_,\
                udev_info->action_,\
                udev_info->subsystem_,\
                udev_info->devtype_,\
                udev_info->vendor_id_,\
                udev_info->product_id_,\
                udev_info->manufacturer_,\
                udev_info->product_,\
                udev_info->serial_,\
                udev_info->busnum_,\
                udev_info->devnum_
        );
    }
}

void UsbManager::print_new_udev_info(const std::shared_ptr<UsbDevice> &udev) {
    auto udev_info = udev->get_all_udevs_info();
    LOG_INFO("\n\
                SYSPATH={}\n\
                DEVNODE={}\n\
                ACTION={}\n\
                SUBSYSTEM={}\n\
                DEVTYPE={}\n\
                VENDOR_ID={}\n\
                PRODUCT_ID={}\n\
                MANUFACTURER={}\n\
                PRODUCT={}\n\
                SERIAL_={}\n\
                BUSNUM={}\n\
                DEVNUM={}\n", \
                udev_info->syspath_,\
                udev_info->devnode_,\
                udev_info->action_,\
                udev_info->subsystem_,\
                udev_info->devtype_,\
                udev_info->vendor_id_,\
                udev_info->product_id_,\
                udev_info->manufacturer_,\
                udev_info->product_,\
                udev_info->serial_,\
                udev_info->busnum_,\
                udev_info->devnum_
        );
}

void UsbManager::add_udev(std::shared_ptr<UsbDevice> udev) {
    udevs_.emplace_back(std::move(udev));
}

void UsbManager::remove_udev(std::shared_ptr<UsbDevice> udev_b) {
    udevs_.erase(
        remove_if(udevs_.begin(), udevs_.end(),
            [&udev_b](std::shared_ptr<UsbDevice> udev_a) {
                return
                udev_a->get_syspath() == udev_b->get_syspath() &&
                udev_a->get_devnode() == udev_b->get_devnode();
            }
        ),
        udevs_.end()
    );
}

int UsbManager::get_udev_size() {
    return udevs_.size();
}