#pragma once
#include "common.hpp"
#include "producer.hpp"
#include "consumer.hpp"
#include "tsProductsQueue.hpp"

enum class ProductType {
    INT,
    STRING
};

enum class Role {
    PRODUCER,
    CONSUMER
};

class ProducerConsumer {
public:
    ProducerConsumer();
    ~ProducerConsumer();

public:
    bool start(Role role, ProductType product_type);
    void stop(Role role, ProductType product_type);
    void stop_all();
    int get_queue_size(ProductType product_type);
    
private:
    Producer producer;
    Consumer consumer;
    std::shared_ptr<ProductsQueue<int>> int_products_queue_ptr;
    std::shared_ptr<ProductsQueue<std::string>> string_products_queue_ptr;
    std::vector<std::pair<Role, ProductType>> product_types;
};