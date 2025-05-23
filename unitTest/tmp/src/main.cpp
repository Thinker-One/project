#include <vector>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <regex>
#include <filesystem>

#include <libudev.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/mount.h>
#include <cstring>
#include <cstdlib>

// Compile with: g++ safe_remove_usb.cpp -o safe_remove_usb -ludev
// Usage: sudo ./safe_remove_usb 1-4.4.4
// Must run as root.

// Find a udev device by subsystem, devtype, and sysname
udev_device* find_udev_device(udev* udev_ctx, const std::string& subsystem,
                               const std::string& devtype,
                               const std::string& sysname) {
    udev_enumerate* enumerate = udev_enumerate_new(udev_ctx);
    udev_enumerate_add_match_subsystem(enumerate, subsystem.c_str());
    udev_enumerate_scan_devices(enumerate);
    udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    for (udev_list_entry* entry = devices; entry; entry = udev_list_entry_get_next(entry)) {
        const char* path = udev_list_entry_get_name(entry);
        udev_device* dev = udev_device_new_from_syspath(udev_ctx, path);
        if (!dev) continue;
        const char* dt = udev_device_get_devtype(dev);
        const char* sn = udev_device_get_sysname(dev);
        if (dt && sn && subsystem == udev_device_get_subsystem(dev) && devtype == dt && sysname == sn) {
            udev_enumerate_unref(enumerate);
            return dev;
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    return nullptr;
}

// Enumerate all block disk devices whose USB parent matches usb_dev
std::vector<udev_device*> get_usb_block_disks(udev* udev_ctx, udev_device* usb_dev) {
    std::vector<udev_device*> disks;
    udev_enumerate* enum_blk = udev_enumerate_new(udev_ctx);
    udev_enumerate_add_match_subsystem(enum_blk, "block");
    udev_enumerate_add_match_property(enum_blk, "ID_BUS", "usb");
    udev_enumerate_scan_devices(enum_blk);
    udev_list_entry* devs = udev_enumerate_get_list_entry(enum_blk);
    const char* usb_syspath = udev_device_get_syspath(usb_dev);
    for (udev_list_entry* entry = devs; entry; entry = udev_list_entry_get_next(entry)) {
        const char* blk_path = udev_list_entry_get_name(entry);
        udev_device* blk = udev_device_new_from_syspath(udev_ctx, blk_path);
        if (!blk) continue;
        const char* dt = udev_device_get_devtype(blk);
        if (!dt || std::string(dt) != "disk") {
            udev_device_unref(blk);
            continue;
        }
        // Check USB parent
        udev_device* parent = udev_device_get_parent_with_subsystem_devtype(
            blk, "usb", "usb_device");
        if (parent && strcmp(udev_device_get_syspath(parent), usb_syspath) == 0) {
            disks.push_back(blk);
        } else {
            udev_device_unref(blk);
        }
    }
    udev_enumerate_unref(enum_blk);
    return disks;
}

// Unmount all partitions under a disk
void unmount_partitions(udev* udev_ctx, udev_device* disk) {
    udev_enumerate* enum_part = udev_enumerate_new(udev_ctx);
    udev_enumerate_add_match_subsystem(enum_part, "block");
    udev_enumerate_scan_devices(enum_part);
    udev_list_entry* parts = udev_enumerate_get_list_entry(enum_part);
    const char* disk_sysname = udev_device_get_sysname(disk);
    for (udev_list_entry* entry = parts; entry; entry = udev_list_entry_get_next(entry)) {
        const char* path = udev_list_entry_get_name(entry);
        udev_device* part = udev_device_new_from_syspath(udev_ctx, path);
        if (!part) continue;
        const char* dt = udev_device_get_devtype(part);
        if (!dt || std::string(dt) != "partition") {
            udev_device_unref(part);
            continue;
        }
        // Check parent disk sysname
        udev_device* parent = udev_device_get_parent_with_subsystem_devtype(
            part, "block", "disk");
        if (parent && strcmp(udev_device_get_sysname(parent), disk_sysname) == 0) {
            const char* devnode = udev_device_get_devnode(part);
            if (devnode) {
                std::cout << "Unmounting " << devnode << "...\n";
                if (umount(devnode) != 0) perror("umount");
            }
        }
        udev_device_unref(part);
    }
    udev_enumerate_unref(enum_part);
}

// Issue SCSI delete for block device
void delete_scsi_device(const std::string& sysname) {
    std::string path = "/sys/block/" + sysname + "/device/delete";
    std::ofstream ofs(path);
    if (!ofs) {
        std::cerr << "Failed to open " << path << "\n";
        return;
    }
    ofs << "1";
    std::cout << "Deleted SCSI device " << sysname << "\n";
}

// Disable the USB device via authorized
void disable_usb(const std::string& usb_syspath) {
    std::string auth = usb_syspath + std::string("/authorized");
    std::ofstream ofs(auth);
    if (!ofs) {
        std::cerr << "Failed to open " << auth << "\n";
        return;
    }
    ofs << "0";
    std::cout << "USB device disabled.\n";
}

int main(int argc, char* argv[]) {
    if (geteuid() != 0) {
        std::cerr << "Must run as root.\n";
        return 1;
    }
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <usb-sysname e.g. 1-4.4.4>\n";
        return 1;
    }
    std::string usb_sysname = argv[1];

    udev* udev_ctx = udev_new();
    if (!udev_ctx) {
        std::cerr << "Failed to initialize udev.\n";
        return 1;
    }
    // Locate USB device
    udev_device* usb_dev = find_udev_device(udev_ctx, "usb", "usb_device", usb_sysname);
    if (!usb_dev) {
        std::cerr << "USB device not found: " << usb_sysname << "\n";
        udev_unref(udev_ctx);
        return 1;
    }
    const char* usb_syspath = udev_device_get_syspath(usb_dev);
    std::cout << "USB device found at " << usb_syspath << "\n";

    // Find block disks under this USB device
    auto disks = get_usb_block_disks(udev_ctx, usb_dev);
    std::cout << "Found " << disks.size() << " block disk(s) under this USB device.\n";
    for (auto& d : disks) {
        std::string name = udev_device_get_sysname(d);
        std::cout << "Processing block disk: " << name << "\n";
        unmount_partitions(udev_ctx, d);
        delete_scsi_device(name);
        udev_device_unref(d);
    }
    // Finally disable USB
    // disable_usb(usb_syspath);

    udev_device_unref(usb_dev);
    udev_unref(udev_ctx);
    std::cout << "Safe removal complete.\n";
    return 0;
}

