#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

#include <cstdint>

enum class MessageType : uint8_t {
    REGISTER,
    ECHO_MSG,
    TURN_TOKEN,
    HAND_DECK,
    TOP_CARD,
    PLACE,
    DRAW,
    SKIP,
};

#endif