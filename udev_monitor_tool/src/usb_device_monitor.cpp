#include "usb_device_monitor.hpp"

UsbDeviceMonitor::UsbDeviceMonitor() : timeout_ms_(2000) {
    running_.store(false);
}

UsbDeviceMonitor::~UsbDeviceMonitor() {
    stop();
}

int UsbDeviceMonitor::init() {
    return 0;
}

int UsbDeviceMonitor::start() {
    running_.store(true);
    usb_dev_mon_thd_ptr_ = std::make_shared<std::thread>(&UsbDeviceMonitor::listen_usb_device_hotplug, this);
    return 0;
}

int UsbDeviceMonitor::stop() {
    if (running_) running_.store(false);
    if (usb_dev_mon_thd_ptr_ && usb_dev_mon_thd_ptr_->joinable()) {
        usb_dev_mon_thd_ptr_->join();
    }
    return 0;
}

std::shared_ptr<UsbDevice> UsbDeviceMonitor::parse_usb_device(UsbCommonTyps::UsbDeviceMapPtr devs, struct udev_device *dev) {

    if (!devs) {
        LOG_ERROR("devs is nullptr!");
        return nullptr;
    }

    auto get = [&](const char *v) -> std::string {
        return v ? v : "";
    };

    std::string dev_syspath = get(udev_device_get_syspath(dev));
    std::shared_ptr<UsbDevice> tmp_device;
    try {
        tmp_device = devs->count(dev_syspath) ? devs->at(dev_syspath) : std::make_shared<UsbDevice>();
    } catch (const std::exception &e) {
        LOG_ERROR("error={}", e.what());
    }

    if (!tmp_device) {
        LOG_ERROR("tmp_device is nullptr!");
        return nullptr;
    }

    tmp_device->set_usb_dev_state(UsbDevice::UsbDeviceState::INIT);
    tmp_device->set_syspath(dev_syspath);
    tmp_device->set_sysname(get(udev_device_get_sysname(dev)));
    tmp_device->set_sysnum(get(udev_device_get_sysnum(dev)));
    tmp_device->set_subsystem(get(udev_device_get_subsystem(dev)));
    tmp_device->set_devpath(get(udev_device_get_devpath(dev)));
    tmp_device->set_devnode(get(udev_device_get_devnode(dev)));
    tmp_device->set_devtype(get(udev_device_get_devtype(dev)));
    tmp_device->set_action(get(udev_device_get_action(dev)));
    tmp_device->set_driver(get(udev_device_get_driver(dev)));
    tmp_device->set_vendor_id(get(udev_device_get_sysattr_value(dev, "idVendor")));
    tmp_device->set_product_id(get(udev_device_get_sysattr_value(dev, "idProduct")));
    tmp_device->set_manufacturer(get(udev_device_get_sysattr_value(dev, "manufacturer")));
    tmp_device->set_product(get(udev_device_get_sysattr_value(dev, "product")));
    tmp_device->set_serial(get(udev_device_get_sysattr_value(dev, "serial")));
    const char *busnum = udev_device_get_sysattr_value(dev, "busnum");
    const char *devnum = udev_device_get_sysattr_value(dev, "devnum");
    try {
        if (busnum) tmp_device->set_busnum(atoi(busnum));
        if (devnum) tmp_device->set_devnum(atoi(devnum));
    } catch (const std::exception& e) {
        LOG_ERROR("atoi error:%s", e.what());
    }

    return devs->count(dev_syspath) ? nullptr : tmp_device;    
}

std::shared_ptr<UsbDevice> UsbDeviceMonitor::parse_usb_interface(UsbCommonTyps::UsbDeviceMapPtr devs, struct udev_device *dev) {

    if (!devs) {
        LOG_ERROR("devs is nullptr!");
        return nullptr;
    }

    auto get = [&](const char *v) -> std::string {
        return v ? v : "";
    };

    struct udev_device *parent_dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
    std::string parent_syspath = get(udev_device_get_syspath(parent_dev));
    std::shared_ptr<UsbDevice> tmp_device;
    try {
        tmp_device = devs->count(parent_syspath) ? devs->at(parent_syspath) : std::make_shared<UsbDevice>();
    } catch (const std::exception &e) {
        LOG_ERROR("error={}", e.what());
    }
    
    if (!tmp_device) {
        LOG_ERROR("tmp_device is nullptr!");
        return nullptr;
    }

    if (!tmp_device->interface_ptr_) {
        LOG_ERROR("tmp_device->interface_ptr_ is nullptr!");
        return nullptr;
    }

    tmp_device->interface_ptr_->set_usb_interface_state(UsbInterface::InterfaceState::INIT);
    tmp_device->interface_ptr_->set_syspath(udev_device_get_syspath(dev));
    tmp_device->interface_ptr_->set_sysname(udev_device_get_sysname(dev));
    tmp_device->interface_ptr_->set_subsystem(udev_device_get_subsystem(dev));
    tmp_device->interface_ptr_->set_devpath(udev_device_get_devpath(dev));
    tmp_device->interface_ptr_->set_devtype(udev_device_get_devtype(dev));
    tmp_device->interface_ptr_->set_driver(udev_device_get_driver(dev));
    tmp_device->interface_ptr_->set_authorized(udev_device_get_sysattr_value(dev, "authorized"));
    tmp_device->interface_ptr_->set_bAlternateSetting(udev_device_get_sysattr_value(dev, "bAlternateSetting"));
    tmp_device->interface_ptr_->set_bInterfaceClass(udev_device_get_sysattr_value(dev, "bInterfaceClass"));
    tmp_device->interface_ptr_->set_bInterfaceNumber(udev_device_get_sysattr_value(dev, "bInterfaceNumber"));
    tmp_device->interface_ptr_->set_bInterfaceProtocol(udev_device_get_sysattr_value(dev, "bInterfaceProtocol"));
    tmp_device->interface_ptr_->set_bInterfaceSubClass(udev_device_get_sysattr_value(dev, "bInterfaceProtocol"));
    tmp_device->interface_ptr_->set_bNumEndpoints(udev_device_get_sysattr_value(dev, "bNumEndpoints"));
    tmp_device->interface_ptr_->set_modalias(udev_device_get_sysattr_value(dev, "modalias"));
    tmp_device->interface_ptr_->set_supports_autosuspend(udev_device_get_sysattr_value(dev, "supports_autosuspend"));
    tmp_device->interface_ptr_->set_uevent(udev_device_get_sysattr_value(dev, "uevent"));

    return devs->count(parent_syspath) ? nullptr : tmp_device;
}

void UsbDeviceMonitor::set_callback (UsbCommonTyps::Callbacks &cbs) {
    callbacks_ = cbs;
}

void UsbDeviceMonitor::listen_usb_device_hotplug() {
    
    UsbCommonTyps::UdevDelPtr udev(udev_new());
    if (!udev) {
        LOG_ERROR("udev_new faild!");
        return;
    }

    UsbCommonTyps::UdevMonDelPtr mon(udev_monitor_new_from_netlink(udev.get(), "udev"));
    if (!mon) {
        LOG_ERROR("udev_monitor_new_from_netlink faild!");
        return;
    }

    udev_monitor_filter_add_match_subsystem_devtype(mon.get(), "usb", nullptr);
    udev_monitor_enable_receiving(mon.get());
    int fd = udev_monitor_get_fd(mon.get());

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    while (running_.load()) {
        int ret = poll(&pfd, 1, timeout_ms_);
        if (ret <= 0) {
            if (ret < 0 && errno != EINTR) {
                LOG_ERROR("poll fatal error, ret={}", ret);
                break;
            }
            continue;
        }

        if (pfd.revents & POLLIN) {
            UsbCommonTyps::UdevDeviceDelPtr dev(udev_monitor_receive_device(mon.get()));
            if (!dev) {
                LOG_ERROR("udev_monitor_receive_device faild!");
                continue;
            }

            std::shared_ptr<UsbDevice> device = parse_usb_device(callbacks_.get_devs_ptr_cb(), dev.get());
            if (device && device->get_devtype() == "usb_device") {
                if (device->get_action() == "add") {
                    LOG_INFO("发现新 USB 设备, UDEV COUNT BEFORE ADD: {}.", callbacks_.get_usb_dev_number_cb());
                    callbacks_.p_usb_dev_info_cb(device);
                    callbacks_.add_usb_dev_cb(device);
                    LOG_INFO("UDEV COUNT AFTER ADD:{}.", callbacks_.get_usb_dev_number_cb());
                } else if (device->get_action() == "remove") {
                    LOG_INFO("移除 USB 设备, UDEV COUNT BEFORE DEL: {}.", callbacks_.get_usb_dev_number_cb());
                    callbacks_.p_usb_dev_info_cb(device);
                    callbacks_.remove_usb_dev_cb(device);
                    LOG_INFO("UDEV COUNT AFTER DEL:{}.", callbacks_.get_usb_dev_number_cb());
                }
            }
        }
    }
    LOG_WARN("listen_usb_device_hotplug thd exit, running_={}", running_.load());
}