#include "usb_monitor.hpp"

UsbMonitor::UsbMonitor() : running_(false), timeout_ms_(2000) {
    
}

UsbMonitor::~UsbMonitor() {
    stop();
}

int UsbMonitor::init() {
    return 0;
}

int UsbMonitor::start() {
    running_ = true;
    monitor_thread_ptr_ = std::make_shared<std::thread>(&UsbMonitor::listen_usb_hotplug, this);
    return 0;
}

int UsbMonitor::stop() {
    if (running_) running_ = false;
    if (monitor_thread_ptr_ && monitor_thread_ptr_->joinable()) {
        monitor_thread_ptr_->join();
    }
    return 0;
}

std::shared_ptr<UsbDevice> UsbMonitor::parse_udev(struct udev_device *dev) {

    UsbDevice device;
    auto get = [&](const char *v) -> std::string {
        return v ? v : "";
    };

    device.set_syspath(get(udev_device_get_syspath(dev)));
    device.set_sysname(get(udev_device_get_sysname(dev)));
    device.set_sysnum(get(udev_device_get_sysnum(dev)));
    device.set_subsystem(get(udev_device_get_subsystem(dev)));
    device.set_devpath(get(udev_device_get_devpath(dev)));
    device.set_devnode(get(udev_device_get_devnode(dev)));
    device.set_devtype(get(udev_device_get_devtype(dev)));
    device.set_action(get(udev_device_get_action(dev)));
    device.set_driver(get(udev_device_get_driver(dev)));

    /* 对 USB 设备，一定要拿 usb_device 这一层 */
    struct udev_device *usb_dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
    if (!usb_dev) usb_dev = dev;  // 枚举时可能本身就是 usb_device

    device.set_vendor_id(get(udev_device_get_sysattr_value(dev, "idVendor")));
    device.set_product_id(get(udev_device_get_sysattr_value(dev, "idProduct")));
    device.set_manufacturer(get(udev_device_get_sysattr_value(dev, "manufacturer")));
    device.set_product(get(udev_device_get_sysattr_value(dev, "product")));
    device.set_serial(get(udev_device_get_sysattr_value(dev, "serial")));

    const char *busnum = udev_device_get_sysattr_value(dev, "busnum");
    const char *devnum = udev_device_get_sysattr_value(dev, "devnum");
    try {
        if (busnum) device.set_busnum(atoi(busnum));
        if (devnum) device.set_devnum(atoi(devnum));
    } catch (const std::exception& e) {
        LOG_ERROR("atoi error:%s", e.what());
    }

    return std::make_shared<UsbDevice>(std::move(device));
}

void UsbMonitor::set_callback (UsbCommonTyps::Callbacks &cbs) {
    callbacks_ = cbs;
}

void UsbMonitor::listen_usb_hotplug() {
    struct udev *udev = udev_new();
    if (!udev) return;

    struct udev_monitor *mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", nullptr);
    udev_monitor_enable_receiving(mon);
    int fd = udev_monitor_get_fd(mon);

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    while (running_) {
        int ret = poll(&pfd, 1, timeout_ms_);
        if (ret <= 0) {
            if (ret < 0 && errno != EINTR) {
                LOG_ERROR("poll fatal error, ret={}", ret);
                break;
            }
            continue;
        }

        if (pfd.revents & POLLIN) {
            struct udev_device *dev = udev_monitor_receive_device(mon);
            if (!dev) continue;

            std::shared_ptr<UsbDevice> udev = parse_udev(dev);
            /* 通常只关心 usb_device */
            if (udev->get_devtype() == "usb_device") {
                if (udev->get_action() == "add") {
                    LOG_INFO("发现新 USB 设备, UDEV COUNT BEFORE ADD: {}.", callbacks_.get_udev_size_cb());
                    callbacks_.p_udev_info_cb(udev);
                    callbacks_.add_udev_cb(udev);
                    LOG_INFO("UDEV COUNT AFTER ADD:{}.", callbacks_.get_udev_size_cb());
                } else if (udev->get_action() == "remove") {
                    LOG_INFO("移除 USB 设备, UDEV COUNT BEFORE DEL: {}.", callbacks_.get_udev_size_cb());
                    callbacks_.p_udev_info_cb(udev);
                    callbacks_.remove_udev_cb(udev);
                    LOG_INFO("UDEV COUNT AFTER DEL:{}.", callbacks_.get_udev_size_cb());
                }
            }
            udev_device_unref(dev);
        }
    }

    udev_monitor_unref(mon);
    udev_unref(udev);
}