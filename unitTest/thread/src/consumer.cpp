#include "consumer.hpp"

Consumer::Consumer() : is_running_int(true), is_running_string(true) {
    
}
Consumer::~Consumer() {
    if (consumer_int_thread_ptr && consumer_int_thread_ptr->joinable()) {
        consumer_int_thread_ptr->join();
    }
    if (consumer_string_thread_ptr && consumer_string_thread_ptr->joinable()) {
        consumer_string_thread_ptr->join();
    }
}
bool Consumer::start_consumer_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr) {
    if (!int_products_queue_ptr) {
        LOG_ERROR("int_products_queue_ptr is null");
        return false;
    }
    if (!consumer_int_thread_ptr) {
        consumer_int_thread_ptr = std::make_shared<std::thread>([this, &int_products_queue_ptr] {
            consumer_int(int_products_queue_ptr);
        });
    }
    return true;
 }
bool Consumer::start_consumer_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr) {
    if (!string_products_queue_ptr) {
        LOG_ERROR("string_products_queue_ptr is null");
        return false;
    }
    if (!consumer_string_thread_ptr) {
        consumer_string_thread_ptr = std::make_shared<std::thread>([this, &string_products_queue_ptr] {
            this->consumer_string(string_products_queue_ptr);
        });
    }
    return true;
}
bool Consumer::start_consumer_all() {
    return true;
}
void Consumer::consumer_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr) {
    while (is_running_int) {
        int product = -1;
        if (int_products_queue_ptr->wait_and_pop(product)) {
            LOG_INFO("Consumer product, int:{}", product);
        }
    }
    LOG_INFO("consumer_int thread exit");
}
void Consumer::consumer_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr) {
    while (is_running_int) {
        std::string product = "";
        if (string_products_queue_ptr->wait_and_pop(product)) {
            LOG_INFO("Consumer product, string:{}", product);
        }
    }
    LOG_INFO("consumer_string thread exit");
}
void Consumer::stop_consumer_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr) {
    int_products_queue_ptr->stop_waiting();
    is_running_int = false;
}
void Consumer::stop_consumer_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr) {
    string_products_queue_ptr->stop_waiting();
    is_running_string = false;
}
void Consumer::stop_consumer_all() {
    is_running_int = false;
    is_running_string = false;
}