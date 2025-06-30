#ifndef QUEUELIST_HPP
#define QUEUELIST_HPP

#include <map>
#include <memory>
#include "Pipe.hpp"

namespace EventStream {

    class QueueList {
    private:
        std::map<Message::Priority, Pipe> queues; // Map of priority to Pipe
        mutable std::mutex mx;                          // Mutex for semaphore
        std::condition_variable cv;         // Condition variable for semaphore
        size_t messageCount = 0;                 // Semaphore counter
        bool end = false;

        bool isEnd() {
            return messageCount == 0 && end;
        }

    public:
        QueueList() = default;
        ~QueueList() = default;

        // Disable copy and assignment
        QueueList(const QueueList&) = delete;
        QueueList& operator=(const QueueList&) = delete;

        // Allow move semantics
        QueueList(QueueList&&) = default;
        QueueList& operator=(QueueList&&) = default;

        // Push a message into the appropriate priority queue
        void push(Message::Priority priority, std::unique_ptr<Message> message) {
            std::lock_guard<std::mutex> lock(mx);
            if (isEnd()) return; // no new messages can be sent
            queues[priority].push(std::move(message));
            ++messageCount;
            cv.notify_one();
        }

        // Pop a message from the highest priority non-empty queue
        std::unique_ptr<Message> pop() {
            std::unique_lock<std::mutex> lock(mx);
            cv.wait(lock, [this] { return messageCount > 0 || end; });
            if (isEnd()) {
                return nullptr; // No messages available
            }

            for (auto it = queues.rbegin(); it != queues.rend(); ++it) { // Iterate from highest to lowest priority
                if (!it->second.empty()) {
                    --messageCount;
                    return it->second.pop();
                }
            }

            throw std::runtime_error("QueueList is empty"); // No messages available (should never happen)
        }

        // Non-blocking pop (returns std::nullopt if no message is available)
        std::unique_ptr<Message> tryPop() {
            std::lock_guard<std::mutex> lock(mx);
            if (isEnd()) {
                return nullptr; // No messages available
            }

            for (auto it = queues.rbegin(); it != queues.rend(); ++it) { // Iterate from highest to lowest priority
                if (!it->second.empty()) {
                    --messageCount;
                    return it->second.tryPop();
                }
            }

            return nullptr; // No messages available (should never happen)
        }

        // Check if all queues are empty
        bool empty() const {
            std::lock_guard<std::mutex> lock(mx);
            return messageCount == 0;
        }

        void notifyEnd() {
            std::lock_guard<std::mutex> lock(mx);
            end = true;
            cv.notify_all();
        }
    };

}

#endif // QUEUELIST_HPP
