#ifndef EVENT_STREAM_GATEWAY_HPP
#define EVENT_STREAM_GATEWAY_HPP

#include "../Utility/Message.hpp"
#include "../Utility/Pipe.hpp"
#include "../Utility/QueueList.hpp"
#include <unordered_map>
#include <list>
#include <typeindex>
#include <thread>

namespace EventStream {

    class Gateway {
    private:
        std::unordered_map<std::type_index, std::list<std::shared_ptr<Pipe>>> pipes; //key: classType (Message child), value: list of pipes to forward the data.
        QueueList &queues;
        std::thread worker;

        void workerMethod();

    public:
        Gateway(QueueList &queues);  // Constructor
        ~Gateway(); // Destructor
    };

}
#endif // EVENT_STREAM_GATEWAY_HPP
