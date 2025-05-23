#include "producerConsumer.hpp"

int main() {
    ProducerConsumer producer_consumer;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    producer_consumer.stop_all();
    return 0;
}