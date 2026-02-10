#include <spdlog.hpp>
#include <poll.h>
#include "usb_device_monitor.hpp"

UsbCommonTyps::Callbacks UsbDeviceMonitor::callbacks_;

UsbDeviceMonitor::UsbDeviceMonitor() : running_(true), timeout_ms_(2000)
{
    usb_dev_mon_thd_ptr_ = std::make_shared<std::thread>(&UsbDeviceMonitor::listen_usb_device_hotplug, this);
}

UsbDeviceMonitor::~UsbDeviceMonitor()
{
    if (usb_dev_mon_thd_ptr_ && usb_dev_mon_thd_ptr_->joinable()) {
        usb_dev_mon_thd_ptr_->join();
    }
}

int UsbDeviceMonitor::start() {
    // TODO
    return 0;
}

int UsbDeviceMonitor::stop() {
    running_.store(false);
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

    std::string syspath = get(udev_device_get_syspath(dev));
    std::shared_ptr<UsbDevice> tmp_device;
    try {
        tmp_device = devs->count(syspath) ? devs->at(syspath) : std::make_shared<UsbDevice>();
    } catch (const std::exception &e) {
        LOG_ERROR("error={}", e.what());
    }

    tmp_device->set_action(get(udev_device_get_action(dev)));
    if (tmp_device->get_usb_dev_state() == UsbDevice::UsbDeviceState::INIT) return tmp_device;

    tmp_device->set_usb_dev_state(UsbDevice::UsbDeviceState::INIT);
    tmp_device->set_syspath(syspath);
    tmp_device->set_sysname(get(udev_device_get_sysname(dev)));
    tmp_device->set_sysnum(get(udev_device_get_sysnum(dev)));
    tmp_device->set_subsystem(get(udev_device_get_subsystem(dev)));
    tmp_device->set_devpath(get(udev_device_get_devpath(dev)));
    tmp_device->set_devnode(get(udev_device_get_devnode(dev)));
    tmp_device->set_devtype(get(udev_device_get_devtype(dev)));
    tmp_device->set_driver(get(udev_device_get_driver(dev)));
    tmp_device->set_vendor_id(get(udev_device_get_sysattr_value(dev, "idVendor")));
    tmp_device->set_product_id(get(udev_device_get_sysattr_value(dev, "idProduct")));
    tmp_device->set_manufacturer(get(udev_device_get_sysattr_value(dev, "manufacturer")));
    tmp_device->set_product(get(udev_device_get_sysattr_value(dev, "product")));
    tmp_device->set_serial(get(udev_device_get_sysattr_value(dev, "serial")));
    tmp_device->set_vendor_id(get(udev_device_get_sysattr_value(dev, "idVendor")));
    tmp_device->set_removable(get(udev_device_get_sysattr_value(dev, "removable")));
    tmp_device->set_maxchild(get(udev_device_get_sysattr_value(dev, "maxchild")));
    const char *busnum = udev_device_get_sysattr_value(dev, "busnum");
    const char *devnum = udev_device_get_sysattr_value(dev, "devnum");
    try {
        if (busnum) tmp_device->set_busnum(atoi(busnum));
        if (devnum) tmp_device->set_devnum(atoi(devnum));
    } catch (const std::exception& e) {
        LOG_ERROR("atoi error:%s", e.what());
    }

    return tmp_device;
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

    std::string syspath = get(udev_device_get_syspath(dev));
    if (tmp_device->interfaces_.count(syspath)) return tmp_device;

    std::shared_ptr<UsbInterface> interface_ptr = std::make_shared<UsbInterface>();
    interface_ptr->set_syspath(syspath);
    interface_ptr->set_sysname(get(udev_device_get_sysname(dev)));
    interface_ptr->set_subsystem(get(udev_device_get_subsystem(dev)));
    interface_ptr->set_devpath(get(udev_device_get_devpath(dev)));
    interface_ptr->set_devtype(get(udev_device_get_devtype(dev)));
    interface_ptr->set_driver(get(udev_device_get_driver(dev)));
    interface_ptr->set_authorized(get(udev_device_get_sysattr_value(dev, "authorized")));
    interface_ptr->set_bAlternateSetting(get(udev_device_get_sysattr_value(dev, "bAlternateSetting")));
    interface_ptr->set_bInterfaceClass(get(udev_device_get_sysattr_value(dev, "bInterfaceClass")));
    interface_ptr->set_bInterfaceNumber(get(udev_device_get_sysattr_value(dev, "bInterfaceNumber")));
    interface_ptr->set_bInterfaceProtocol(get(udev_device_get_sysattr_value(dev, "bInterfaceProtocol")));
    interface_ptr->set_bInterfaceSubClass(get(udev_device_get_sysattr_value(dev, "bInterfaceSubClass")));
    interface_ptr->set_bNumEndpoints(get(udev_device_get_sysattr_value(dev, "bNumEndpoints")));
    interface_ptr->set_modalias(get(udev_device_get_sysattr_value(dev, "modalias")));
    interface_ptr->set_supports_autosuspend(get(udev_device_get_sysattr_value(dev, "supports_autosuspend")));
    interface_ptr->set_uevent(get(udev_device_get_sysattr_value(dev, "uevent")));
    UsbCommonTyps::InterfaceDescriptor descriptor = {
        interface_ptr->get_bInterfaceClass(),
        interface_ptr->get_bInterfaceSubClass(),
        interface_ptr->get_bInterfaceProtocol()
    };
    tmp_device->set_devname(descriptor, callbacks_.get_num_of_various_dev_cb());
    tmp_device->interfaces_.emplace(syspath, interface_ptr);
    tmp_device->set_latest_interface(interface_ptr);
    return tmp_device;
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
          
            std::string devtype = udev_device_get_devtype(dev.get());

            if (devtype == "usb_device") {
                std::shared_ptr<UsbDevice> device_ptr = parse_usb_device(callbacks_.get_devs_ptr_cb(), dev.get());
                if (device_ptr && device_ptr->get_action() == "add") {
                    LOG_WARN("新的USB设备");
                    // LOG_DEBUG("添加前:{}", callbacks_.get_usb_dev_number_cb());
                    callbacks_.p_usb_dev_info_cb(device_ptr);
                    callbacks_.add_usb_dev_cb(device_ptr);
                }
                if (device_ptr && device_ptr->get_action() == "remove") {
                    LOG_WARN("移除USB设备");
                    // LOG_DEBUG("移除前:{}", callbacks_.get_usb_dev_number_cb());
                    callbacks_.p_usb_dev_info_cb(device_ptr);
                    callbacks_.remove_usb_dev_cb(device_ptr);
                }
            }

            if (devtype == "usb_interface") {
                std::shared_ptr<UsbDevice> device_ptr = parse_usb_interface(callbacks_.get_devs_ptr_cb(), dev.get());
                if (device_ptr) {
                    if (device_ptr && device_ptr->get_action() == "add") {
                        LOG_WARN("新的USB设备接口");
                        callbacks_.p_interface_info_cb(device_ptr->get_latest_interface());
                        // LOG_DEBUG("添加后:{}", callbacks_.get_usb_interface_number_cb());
                    }
                }
            }
        }
    }
    LOG_WARN("listen_usb_device_hotplug thd exit, running_={}", running_.load());
}