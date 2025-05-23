#pragma once
#include <random>
#include "common.hpp"
#include "tsProductsQueue.hpp"

class Producer
{
public:
    Producer();
    ~Producer();

public:
    bool start_generate_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr);
    bool start_generate_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr);
    bool start_generate_all();

    void generate_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr);
    void generate_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr);
    
    void stop_generate_int();
    void stop_generate_string();
    void stop_generate_all();

private:
    std::shared_ptr<std::thread> produce_int_thread_ptr, produce_string_thread_ptr;
    std::atomic_bool is_running_int, is_running_string;
    
};