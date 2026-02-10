#include "common.hpp"
#include "proc_watcher.hpp"

int main() {
    ProcWatcher proc_watcher;
    proc_watcher.init();
    proc_watcher.start();
    while (true) {
        CommonTool::run_cmd("pwd");
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}