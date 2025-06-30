#ifndef PIPE_HPP
#define PIPE_HPP

#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "Message.hpp"

namespace EventStream {

    class Pipe {
    private:
        std::deque<std::unique_ptr<Message>> queue;
        mutable std::mutex mx;
        std::condition_variable cv;

    public:
        Pipe() = default;
        ~Pipe() = default;

        // Disable copy and assignment
        Pipe(const Pipe&) = delete;
        Pipe& operator=(const Pipe&) = delete;

        // Allow move semantics
        Pipe(Pipe&&) = default;
        Pipe& operator=(Pipe&&) = default;

        // Push a message into the pipe
        void push(std::unique_ptr<Message> message) {
            {
                std::lock_guard<std::mutex> lock(mx);
                queue.push_back(std::move(message));
            }
            cv.notify_one();
        }

        // Pop a message from the pipe; blocks until a message is available
        std::unique_ptr<Message> pop() {
            std::unique_lock<std::mutex> lock(mx);
            cv.wait(lock, [this] { return !queue.empty(); });
            auto message = std::move(queue.front());
            queue.pop_front();
            return message;
        }

        // Non-blocking pop (returns nullptr if no message is available)
        std::unique_ptr<Message> tryPop() {
            std::lock_guard<std::mutex> lock(mx);
            if (queue.empty()) {
                return nullptr;
            }
            auto message = std::move(queue.front());
            queue.pop_front();
            return message;
        }

        // Check if the pipe is empty
        bool empty() const {
            std::lock_guard<std::mutex> lock(mx);
            return queue.empty();
        }
    };

}

#endif // PIPE_HPP
