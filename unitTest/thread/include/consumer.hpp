#pragma once
#include "common.hpp"
#include "tsProductsQueue.hpp"

class Consumer {
public:
    Consumer();
    ~Consumer();

public:
    bool start_consumer_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr);
    bool start_consumer_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr);
    bool start_consumer_all();

    void consumer_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr);
    void consumer_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr);

    void stop_consumer_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr);
    void stop_consumer_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr);
    void stop_consumer_all();

private:
    std::shared_ptr<std::thread> consumer_int_thread_ptr, consumer_string_thread_ptr;
    std::atomic_bool is_running_int, is_running_string;
};