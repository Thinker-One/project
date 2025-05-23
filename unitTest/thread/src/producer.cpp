#include "producer.hpp"

Producer::Producer() : is_running_int(true), is_running_string(true) {
   
}

Producer::~Producer() {
    if (produce_int_thread_ptr && produce_int_thread_ptr->joinable()) {
        produce_int_thread_ptr->join();
    }
    if (produce_string_thread_ptr && produce_string_thread_ptr->joinable()) {
        produce_string_thread_ptr->join();
    }
}

bool Producer::start_generate_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr) {
    if (!int_products_queue_ptr) {
        LOG_ERROR("int_products_queue_ptr is null");
        return false;
    }
    if (!produce_int_thread_ptr) {
        // lambda 按值捕获 int_products_queue_ptr，按引用捕获要注意 int_products_queue_ptr 生命周期
        // 否则可能会导致悬空引用
        produce_int_thread_ptr = std::make_shared<std::thread>([this, &int_products_queue_ptr] {
            generate_int(int_products_queue_ptr);
        });
    }
    return true;
}
bool Producer::start_generate_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr) {
    if (!string_products_queue_ptr) {
        LOG_ERROR("string_products_queue_ptr is null");
        return false;
    }
    if (!produce_string_thread_ptr) {
        // 这种写法代码更复杂，适合高频调用的场景。引用计数不增加，需显式检查有效性。
        std::weak_ptr<ProductsQueue<std::string>> weak_ptr = string_products_queue_ptr;
        produce_string_thread_ptr = std::make_shared<std::thread>([this, weak_ptr] {
            if (auto ptr = weak_ptr.lock()) {   // 尝试提升为 shared_ptr
                this->generate_string(ptr);
            }
        });
    }
    return true;
}
bool Producer::start_generate_all() {
    // if (!produce_int_thread_ptr) {
    //     produce_int_thread_ptr = std::make_shared<std::thread>(&Producer::generate_int, this);
    // }
    // if (!produce_string_thread_ptr) {
    //     produce_string_thread_ptr = std::make_shared<std::thread>(&Producer::generate_string, this);
    // }
    return true;
}

void Producer::generate_int(std::shared_ptr<ProductsQueue<int>>& int_products_queue_ptr) {
    while (is_running_int) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 99);
        int_products_queue_ptr->push(dist(gen));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    LOG_INFO("generate_int thread exit");
}

void Producer::generate_string(std::shared_ptr<ProductsQueue<std::string>>& string_products_queue_ptr) {
    while (is_running_string) {
        const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyz";
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(0, charset.size() - 1);
        std::string result(10, '\0');
        std::generate(result.begin(), result.end(), [&]() {
            return charset[distribution(generator)];
        });
        string_products_queue_ptr->push(result);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    LOG_INFO("generate_string thread exit");
}

void Producer::stop_generate_int() {
    is_running_int = false;
}

void Producer::stop_generate_string() {
    is_running_string = false;
}

void Producer::stop_generate_all() {
    is_running_int = false;
    is_running_string = false;
}