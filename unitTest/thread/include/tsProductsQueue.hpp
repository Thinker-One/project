#pragma once
#include "common.hpp"

template <typename T>
class ProductsQueue {
public:
    ProductsQueue(): max_queue_size(20), is_waiting(true) {

    };
    ~ProductsQueue() {
        
    };
    
    /*
     * 必须使用独立模板参数 U（而非直接使用类的模板参数 T）。否则T&& product就不是万能引用，因为当 ProductsQueue<T> 
     * 实例化后（如 ProductsQueue<std::string>），T&& 会固定为 std::string&&（右值引用），​​无法绑定左值​​。
    */
    
    template <typename U>
    bool push(U&& product) {
        std::lock_guard<std::mutex> lock(products_queue_mtx);
        if (products_queue.size() == max_queue_size) {
            LOG_INFO("Queue is full");
            return false;
        }
        products_queue.push(std::forward<U>(product));
        products_queue_cv.notify_one();
        LOG_INFO("Push product to queue, {}", product);
        return true;
    }
    bool try_pop(T& product) {
        std::lock_guard<std::mutex> lock(products_queue_mtx);
        if (products_queue.size() == 0) {
            LOG_INFO("Queue is empty");
            return false;
        }
        product = products_queue.front();
        products_queue.pop();
        return true;
    }
    bool wait_and_pop(T& product) {
        std::unique_lock<std::mutex> lock(products_queue_mtx);
        products_queue_cv.wait(lock, [this] {
            return !is_waiting || products_queue.size() != 0;
        });
        if (!is_waiting) {
            LOG_INFO("Stop waiting");
            return false;
        }
        product = std::move(products_queue.front());
        products_queue.pop();
        return true;
    }
    void stop_waiting() {
        is_waiting = false;
        products_queue_cv.notify_all();
    }
    bool empty() {
        std::lock_guard<std::mutex> lock(products_queue_mtx);
        return products_queue.size() == 0;
    }
    bool full() {
        std::lock_guard<std::mutex> lock(products_queue_mtx);
        return products_queue.size() == max_queue_size;
    }
    int size() {
        std::lock_guard<std::mutex> lock(products_queue_mtx);
        return products_queue.size();
    }
    T at(unsigned int pos) {
        std::lock_guard<std::mutex> lock(products_queue_mtx);
        if (products_queue.size() == 0) {
            LOG_INFO("Queue is empty");
            throw std::runtime_error("Queue is empty");
        }
        if (pos >= products_queue.size()) {
            LOG_INFO("Queue size exceeded");
            throw std::out_of_range("Position out of range");
        }
        return products_queue.at(pos);
    }
    

private:
    const unsigned int max_queue_size;
    std::queue<T> products_queue;
    std::mutex products_queue_mtx;
    std::condition_variable products_queue_cv;
    std::atomic_bool is_waiting;
};