#include "producerConsumer.hpp"

ProducerConsumer::ProducerConsumer() : producer(), consumer(),
    int_products_queue_ptr(std::make_shared<ProductsQueue<int>>()),
    string_products_queue_ptr(std::make_shared<ProductsQueue<std::string>>()),
    product_types {
        {Role::PRODUCER, ProductType::INT},
        {Role::PRODUCER, ProductType::STRING},
        {Role::CONSUMER, ProductType::INT},
        {Role::CONSUMER, ProductType::STRING}
    } {
    start(Role::PRODUCER, ProductType::INT);
    start(Role::PRODUCER, ProductType::STRING);
    start(Role::CONSUMER, ProductType::INT);
    start(Role::CONSUMER, ProductType::STRING);
}

ProducerConsumer::~ProducerConsumer() {
    // stop(Role::PRODUCER, ProductType::INT);
    // stop(Role::PRODUCER, ProductType::STRING);
    // stop(Role::CONSUMER, ProductType::INT);
    // stop(Role::CONSUMER, ProductType::STRING);
}

bool ProducerConsumer::start(Role role, ProductType product_type) {

    switch (role) {
        case Role::PRODUCER:
        {
            switch (product_type) {
                case ProductType::INT:
                    if (!producer.start_generate_int(int_products_queue_ptr)) {
                        LOG_ERROR("start_generate_int() failed");
                        return false;
                    }
                    break;
                case ProductType::STRING:
                    if (!producer.start_generate_string(string_products_queue_ptr)) {
                        LOG_ERROR("start_generate_string() failed");
                        return false;
                    }
                    break;
                default:
                    break;
            }
            break;
        }    
        case Role::CONSUMER:
        {
            switch (product_type) {
                case ProductType::INT:
                    if (!consumer.start_consumer_int(int_products_queue_ptr)) {
                        LOG_ERROR("start_consumer_int() failed");
                        return false;
                    }
                    break;
                case ProductType::STRING:
                    if (!consumer.start_consumer_string(string_products_queue_ptr)) {
                        LOG_ERROR("start_consumer_string() failed");
                        return false;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return true;
}

void ProducerConsumer::stop(Role role, ProductType product_type) {

    switch (role) {
        case Role::PRODUCER:
        {
            switch (product_type) {
                case ProductType::INT:
                    producer.stop_generate_int();
                    break;
                case ProductType::STRING:
                    producer.stop_generate_string();
                    break;
                default:
                    break;
            }
            break;
        }    
        case Role::CONSUMER:
        {
            switch (product_type) {
                case ProductType::INT:
                    consumer.stop_consumer_int(int_products_queue_ptr);
                    break;
                case ProductType::STRING:
                    consumer.stop_consumer_string(string_products_queue_ptr);
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

void ProducerConsumer::stop_all() {
    for (auto& iter : product_types) {
        stop(iter.first, iter.second);
    }
}
int ProducerConsumer::get_queue_size(ProductType product_type) {
    switch (product_type) {
        case ProductType::INT:
            return int_products_queue_ptr->size();
        case ProductType::STRING:
            return string_products_queue_ptr->size();
        default:
            LOG_ERROR("Invalid product type");
            return -1;
    }
}