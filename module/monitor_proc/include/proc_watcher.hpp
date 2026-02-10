#ifndef PROC_WATCHER_HPP
#define PROC_WATCHER_HPP

#include <unordered_set>
#include <atomic>
#include <thread>


#define MAX_BUFFER_SIZE 4096

class ProcWatcher {
public:
    ProcWatcher();
    ~ProcWatcher();
    bool init();
    int start();
    int stop();

public:
    void print_proc_info(int tgid);

private:
    bool init_nl_socket();
    bool set_proc_ev_listen(bool enable);
    void proc_watcher();
    void handle_event(const struct proc_event &ev);

private:
    std::atomic<bool> running_;
    std::thread watch_thread_;
    int nl_sock_;
    int timeout_ms_;
    std::unordered_set<int> cur_proc_pids_;
};


#endif