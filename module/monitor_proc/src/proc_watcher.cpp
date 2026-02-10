#include <unistd.h>
#include <spdlog.hpp>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <linux/cn_proc.h>
#include <poll.h>
#include <fstream>
#include "proc_watcher.hpp"

ProcWatcher::ProcWatcher() : running_(false), nl_sock_(-1), timeout_ms_(2000) {
    cur_proc_pids_.insert(getpid());
}

ProcWatcher::~ProcWatcher() {
    stop();
}

bool ProcWatcher::init() {

    if (!init_nl_socket()) {
        LOG_ERROR("init_nl_socket failed!");
        return false;
    }

    if (!set_proc_ev_listen(true)) {
        close(nl_sock_);
        nl_sock_ = -1;
        LOG_ERROR("set_proc_ev_listen failed!");
        return false;
    }

    return true;
}

int ProcWatcher::start() {
    running_ = true;
    watch_thread_ = std::thread(&ProcWatcher::proc_watcher, this);
    return 0;
}

int ProcWatcher::stop() {

    if (!running_.exchange(false)) return  -1;

    if (watch_thread_.joinable()) {
        watch_thread_.join();
    }

    return 0;
}

bool ProcWatcher::init_nl_socket() {

    nl_sock_ = socket(PF_NETLINK, SOCK_DGRAM | SOCK_NONBLOCK, NETLINK_CONNECTOR);
    if (nl_sock_ < 0) {
        LOG_ERROR("socket");
        return false;
    }

    sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = CN_IDX_PROC;

    if (bind(nl_sock_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        LOG_ERROR("bind");
        close(nl_sock_);
        nl_sock_ = -1;
        return false;
    }

    return true;
}

bool ProcWatcher::set_proc_ev_listen(bool enable)  {

    if (nl_sock_ < 0) {
        LOG_ERROR("nl_sock < 0");
        return false;
    }

    struct __attribute__((aligned(NLMSG_ALIGNTO))) {
        struct nlmsghdr nl_hdr;
        struct __attribute__((__packed__)) {
            struct cn_msg cn_msg;
            enum proc_cn_mcast_op op;
        } body;
    } msg;

    memset(&msg, 0, sizeof(msg));
    msg.nl_hdr.nlmsg_len = sizeof(msg);
    msg.nl_hdr.nlmsg_type = NLMSG_DONE;
    msg.nl_hdr.nlmsg_pid = getpid();

    msg.body.cn_msg.id.idx = CN_IDX_PROC;
    msg.body.cn_msg.id.val = CN_VAL_PROC;
    msg.body.cn_msg.len = sizeof(enum proc_cn_mcast_op);
    msg.body.op = enable ? PROC_CN_MCAST_LISTEN : PROC_CN_MCAST_IGNORE;

    if (send(nl_sock_, &msg, sizeof(msg), 0) < 0) {
        LOG_ERROR("send");
        return false;
    }

    LOG_INFO("订阅成功");
    return true;
}

#if 0
    struct msghdr {
        // 接收端点地址
        void         *msg_name;             对端地址 
        socklen_t     msg_namelen;          地址长度

        // 整个结构里唯一不可或缺的一部分它告诉内核：用户态缓冲区在哪，有几段（scatter-gather）
        struct iovec *msg_iov;              接收缓冲区地址
        size_t        msg_iovlen;           接收缓冲区大小

        // 辅助数据
        void         *msg_control;
        size_t        msg_controllen;

        // 输出字段，由内核填写
        int           msg_flags;
    };

    // edge
    struct sockaddr_nl addr;
    msg.msg_name = &addr;
    msg.msg_namelen = sizeof(addr);

    // 进阶用法：零拷贝 / 分段
    struct iovec iov[2] = {
        { header, sizeof(header) },
        { payload, sizeof(payload) }
    };
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;

    // Netlink 的消息格式规定：每一个消息都必须以 nlmsghdr (Netlink Message Header) 开头
    struct nlmsghdr
#endif

void ProcWatcher::proc_watcher() {

    struct pollfd pfd;
    pfd.fd = nl_sock_;
    pfd.events = POLLIN;

    std::vector<char> buffer(64 * 1024);
    struct iovec iov;
    struct msghdr msg;

    while (running_) {
        memset(&iov, 0, sizeof(iov));
        memset(&msg, 0, sizeof(msg));

        iov.iov_base = buffer.data();
        iov.iov_len  = buffer.size();

        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        int ret = poll(&pfd, 1, timeout_ms_);
        if (ret <= 0) {
            if (ret < 0 && errno != EINTR) {
                LOG_ERROR("poll fatal error, ret={}", ret);
                break;
            }
            LOG_INFO("已超时");
            continue;
        }

        if (pfd.revents & POLLIN) {
            ssize_t len = recvmsg(nl_sock_, &msg, 0);
            if (len < 0) {
                if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
                    LOG_WARN("recvmsg, errno={}", errno);
                    continue;
                }
                LOG_ERROR("recvmsg, errno={}", errno);
                break;
            }

            for (struct nlmsghdr* nlh = (struct nlmsghdr*)buffer.data(); NLMSG_OK(nlh, len); nlh = NLMSG_NEXT(nlh, len)) {

                // ---- 基础 netlink 消息处理 ----
                if (nlh->nlmsg_type == NLMSG_NOOP) {
                    LOG_INFO("空包");
                    continue;
                }
                    

                if (nlh->nlmsg_type == NLMSG_ERROR) {
                    LOG_ERROR("netlink error message");
                    continue;
                }

                if (nlh->nlmsg_type == NLMSG_DONE) {
                    // LOG_INFO("数据已结束");
                    // continue;
                }

                // ---- connector 消息校验 ----
                if (nlh->nlmsg_len < NLMSG_LENGTH(sizeof(struct cn_msg))) {
                    LOG_WARN("netlink 数据格式错误");
                    continue;
                }

                auto* cn = (struct cn_msg*)NLMSG_DATA(nlh);
                if (cn->len < sizeof(struct proc_event)) {
                    LOG_WARN("proc_event 数据格式错误");
                    continue;
                }

                auto* ev = (struct proc_event*)cn->data;
                handle_event(*ev);
            }
        }
    }

}

void ProcWatcher::handle_event(const struct proc_event &ev) {

    switch (ev.what) {
        case proc_event::PROC_EVENT_FORK: {
            LOG_INFO("\n\
                    [FORK]\n\
                    parent_pid={},\n\
                    parent_tgid={},\n\
                    child_pid={}\n\
                    child_tgid={}",\
                    ev.event_data.fork.parent_pid,\
                    ev.event_data.fork.parent_tgid,\
                    ev.event_data.fork.child_pid,\
                    ev.event_data.fork.child_tgid
            );
            print_proc_info(ev.event_data.exec.process_tgid);
            break;
        }
            
        case proc_event::PROC_EVENT_EXEC: {
            LOG_WARN("\n\
                    [EXEC]\n\
                    process_pid={},\n\
                    process_tgid={}",\
                    ev.event_data.exec.process_pid,\
                    ev.event_data.exec.process_tgid
            );
            print_proc_info(ev.event_data.exec.process_tgid);
            break;
        }
            
        case proc_event::PROC_EVENT_EXIT: {
            LOG_INFO("\n\
                    [EXIT]\n\
                    process_pid={},\n\
                    process_tgid={},\n\
                    exit_code={}\n\
                    exit_signal={}",\
                    ev.event_data.exit.process_pid,\
                    ev.event_data.exit.process_tgid,\
                    ev.event_data.exit.exit_code,\
                    ev.event_data.exit.exit_signal
            );
            print_proc_info(ev.event_data.exec.process_tgid);
            break;
        }

        default: {
            break;
        }
    }
}

void ProcWatcher::print_proc_info(int tgid) {
    pid_t pid = tgid;
        
    // 1. 获取可执行文件路径
    char exe_path[4096];
    std::string proc_exe_link = "/proc/" + std::to_string(pid) + "/exe";
    ssize_t len = readlink(proc_exe_link.c_str(), exe_path, sizeof(exe_path) - 1);
    
    if (len != -1) {
        exe_path[len] = '\0';
        LOG_INFO("[EXEC DETECTED] PID: {}, Path: {}", pid, exe_path);
    }

    // 2. 获取命令行参数
    std::string cmdline_path = "/proc/" + std::to_string(pid) + "/cmdline";
    std::ifstream cmd_file(cmdline_path);
    std::string cmdline;
    if (cmd_file.is_open()) {
        std::getline(cmd_file, cmdline); 
        // 注意：cmdline 中的参数是以 '\0' 分隔的，可以根据需要替换为空格
        for (auto &ch : cmdline) { if (ch == '\0') ch = ' '; }
        LOG_INFO("[CMDLINE] {}", cmdline);
    }
}