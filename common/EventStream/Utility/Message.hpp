#ifndef EVENT_STREAM_MESSAGE_HPP
#define EVENT_STREAM_MESSAGE_HPP

#include <typeinfo>

namespace EventStream {

    class Message {
    public:
        enum class Priority {
            CONFIG = 3,
            CRITICAL = 2,
            HIGH = 1,
            GENERAL = 0,
        };

        virtual const std::type_info &getType() {
            return typeid(*this);
        }
    };

}

#endif // EVENT_STREAM_MESSAGE_HPP
