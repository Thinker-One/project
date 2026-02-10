#ifndef THD_SAFE_QUEUE
#define THD_SAFE_QUEUE
#include <queue>

template <typename T>
class ThdSafeQueue {
public:
    explicit ThdSafeQueue(size_t max_size = 1024) : running_push_(true),
                                                    running_pop_(true),
                                                    max_queue_size_(max_size)
    {}

    ~ThdSafeQueue()
    {
        stop_all();
    }
    
    // 禁止拷贝构造和赋值（线程安全组件通常不应被拷贝）
    ThdSafeQueue(const ThdSafeQueue&) = delete;
    ThdSafeQueue& operator=(const ThdSafeQueue&) = delete;

    #if 0
        必须使用独立模板参数 U (而非直接使用类的模板参数 T)。否则T&& product就不是万能引用, 因为当 ThdSafeQueue<T> 
        实例化后(如 ThdSafeQueue<std::string>), T&& 会固定为 std::string&&(右值引用), ​​无法绑定左值​​。
    #endif
    
    template <typename U>
    bool try_push(U&& product) {
        std::unique_lock<std::mutex> lock(queue_mtx_);
        if (queue_.size() >= max_queue_size_) {
            return false;
        }
        queue_.push(std::forward<U>(product));
        lock.unlock();
        queue_pop_cv_.notify_one();
        return true;
    }

    template <typename U>
    bool block_push(U&& product) {
        std::unique_lock<std::mutex> lock(queue_mtx_);
        queue_push_cv_.wait(lock, [this] {
            return queue_.size() < max_queue_size_ || !running_push_.load(std::memory_order_acquire);
        });

        if (!running_push_.load(std::memory_order_acquire)) {
            return false;
        }

        queue_.push(std::forward<U>(product));
        lock.unlock();
        queue_pop_cv_.notify_one();
        return true;
    }

    bool try_pop(T& product) {
        std::unique_lock<std::mutex> lock(queue_mtx_);
        if (queue_.empty()) {
            return false;
        }
        
        product = std::move(queue_.front());
        queue_.pop();
        lock.unlock();
        queue_push_cv_.notify_one();
        return true;
    }

    bool block_pop(T& product) {
        std::unique_lock<std::mutex> lock(queue_mtx_);
        queue_pop_cv_.wait(lock, [this] {
            return !queue_.empty() || !running_pop_.load(std::memory_order_acquire);
        });

        if (!running_pop_.load(std::memory_order_acquire)) {
            return false;
        }

        product = std::move(queue_.front());
        queue_.pop();
        lock.unlock();
        queue_push_cv_.notify_one();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(queue_mtx_);
        return queue_.empty();
    }

    bool full() const {
        std::lock_guard<std::mutex> lock(queue_mtx_);
        return queue_.size() >= max_queue_size_;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(queue_mtx_);
        return queue_.size();
    }

    void stop_block_push() {
        running_push_.store(false, std::memory_order_release);
        queue_push_cv_.notify_all();
    }

    void stop_block_pop() {
        running_pop_.store(false, std::memory_order_release);
        queue_pop_cv_.notify_all();
    }

    void stop_all() {
        running_push_.store(false, std::memory_order_release);
        running_pop_.store(false, std::memory_order_release);
        queue_push_cv_.notify_all();
        queue_pop_cv_.notify_all();
    }
    

private:
    std::atomic<bool> running_push_;
    std::atomic<bool> running_pop_;
    const size_t max_queue_size_;
    std::queue<T> queue_;
    mutable std::mutex queue_mtx_;
    std::condition_variable queue_pop_cv_;
    std::condition_variable queue_push_cv_;
};

#endif