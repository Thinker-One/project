#include "file_watcher.hpp"

int main() {
    FileWatcher f_watcher;
    // f_watcher.init("/root/zhn/t.cc");
    f_watcher.init();
    f_watcher.start();
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}