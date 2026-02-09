#ifndef FILE_WATCHER_HPP
#define FILE_WATCHER_HPP
#include "common.hpp"

#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>
#include <chrono>
#include <sys/inotify.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <unordered_map>
#include <unordered_set>


#define MAX_BUFFER_SIZE 4096

class FileWatcher {
public:
    using Callback = std::function<void(std::chrono::steady_clock::time_point, const std::string &)>;
    using DirMap = std::vector<std::pair<std::string, std::string>>;
    FileWatcher();
    ~FileWatcher();
    bool init();
    int start();
    int stop();

public:
    void print_inotify_dir_info();

private:
    static void split_path(const std::string& full, std::string& dir, std::string& file);
    void run();
    bool is_vim_noise(const std::string& filename);
    void get_inotify_dir();
    void handle_event(std::chrono::steady_clock::time_point now, const std::string &str);
    void get_user_startup_dirs();
    bool is_monitor_obj(std::string dir, std::string filename);

private:
    int inotify_fd_;
    std::unordered_map<int, std::string> watch_descs_;
    std::unordered_set<std::string> already_inotify_dirs_;
    std::atomic<bool> running_;
    std::thread watch_thread_;
    std::string target_path_;
    std::string dir_path_;
    std::string filename_;
    Callback callback_;
    std::mutex mutex_;
    std::chrono::steady_clock::time_point last_trigger_time_;
    DirMap dirs_;
};


#endif