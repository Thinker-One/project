#ifndef PROC_WATCHER_HPP
#define PROC_WATCHER_HPP

#include <atomic>
#include <thread>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "thd_safe_queue.hpp"


#define MAX_BUFFER_SIZE 4096

struct NewProc {
    int pid;
    int ppid;
};

struct RecodInfo {
    int ppid;
    std::chrono::steady_clock::time_point start_time;
    bool report;
};

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
    void init_consumer();
    void handle_new_proc();
    void handle_exit_proc();
    void proc_watcher();
    void handle_event(const struct proc_event &ev);
    void clean_expired_records(std::chrono::steady_clock::time_point now);

private:
    int nl_sock_;
    int timeout_ms_;
    std::atomic<bool> running_;
    std::chrono::steady_clock::time_point start_time_;

    const size_t max_queue_size_ = 4096;
    const size_t max_childs_size_ = 4096;
    const size_t max_record_size_ = 4096;

    
    std::shared_ptr<std::thread> new_proc_consumer_ptr_;
    std::shared_ptr<std::thread> exit_proc_consumer_ptr_;
    std::thread watch_thread_;

    ThdSafeQueue<NewProc> new_proc_queue_(max_queue_size_);
    ThdSafeQueue<int> exit_proc_queue_;

    std::unordered_set<int> cur_proc_pids_;
    std::unordered_set<int> childs_;
    std::unordered_map<int, RecodInfo> proc_record_;
    // std::mutex new_proc_queue_mtx_;
    // std::mutex exit_proc_queue_mtx_;
    // std::condition_variable new_proc_queue_cv_;
    // std::condition_variable exit_proc_queue_cv_;
    
    
};


#endif