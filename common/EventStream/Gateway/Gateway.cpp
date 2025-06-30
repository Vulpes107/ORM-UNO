#include "Gateway.hpp"

using namespace EventStream;

Gateway::Gateway(QueueList &queues) : queues(queues), worker(&EventStream::Gateway::workerMethod, this) {}

Gateway::~Gateway() {
    worker.join();
}

void Gateway::workerMethod() {
    std::unique_ptr<Message> message;
    while ((message = queues.pop()) != nullptr) {
        if (0) {
            /// TODO: SUBSCRIBE AND UNSUBSCRIBE
        } else {
            std::list<std::shared_ptr<Pipe>> &subscribers = pipes[message->getType()];
            for (auto it = subscribers.begin(); it != subscribers.end(); std::advance(it,1)) {
                (*it)->push(std::make_unique<Message>(*message));
            }
        }
    }
}