#include <sys/socket.h> //socket
#include <fcntl.h>      //for open
#include <unistd.h>     //for close
#include <cassert>      //for assert
#include <stdexcept>
#include <iostream>
#include <cerrno>
#include <clocale>
#include <cstring>
#include <array>

#include <TCP/config.hpp>
#include "Client.hpp"

Client::Client() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock_fd != -1);  // Failed to create a socket

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server.sin_port = htons(DEFAULT_PORT);
}

Client::~Client() {
    if (close(sock_fd) < 0) {
        std::cerr << "Error closing socket" << std::endl;
    }
}

void Client::connectToServer() {
    if (connect(sock_fd, (struct sockaddr*)&server, sizeof(server)) != -1) {
        char *ip = inet_ntoa(server.sin_addr);
        uint16_t port = ntohs(server.sin_port);
        std::cout << "Connected to " << ip << ":" << port << std::endl;
    } else {
        throw std::runtime_error("Unable to connect to server");
    }
}

int Client::receive(void *buf, const size_t size) {
    //Receive a message from server
    int read_size = recv(sock_fd , buf , size , 0);

    if(read_size == 0) {
        throw std::runtime_error("Server disconnected");
    } else if(read_size == -1) {
        throw std::runtime_error(std::strerror(errno));
    } else {
        std::cout << "Received: " << read_size << " bytes" << std::endl;
    }

    return read_size;
}

// Receive overload for MessageType
int Client::receive(MessageType &msgType) {
    uint8_t typeRaw;
    int bytesRead = receive(&typeRaw, sizeof(typeRaw));
    msgType = static_cast<MessageType>(typeRaw);
    return bytesRead;
}

// Receive overload for std::string
int Client::receive(std::string &msg) {
    uint32_t length;
    receive(&length, sizeof(length)); // First receive the length of the string
    length = ntohl(length);          // Convert from network to host byte order

    std::vector<char> buffer(length);
    receive(buffer.data(), length); // Receive the string data
    msg.assign(buffer.begin(), buffer.end());
    return static_cast<int>(length);
}

// Receive overload for Card
int Client::receive(Card &card) {
    std::array<uint8_t, 3> data;
    receive(data.data(), data.size());

    Color color = static_cast<Color>(data[0]);
    Type type = static_cast<Type>(data[1]);
    int number = static_cast<int>(data[2]);

    card = Card(color, type, number);
    return static_cast<int>(data.size());
}

int Client::send(const void *buf, const size_t size) {
    //Send a message to server
    const int write_size = ::send(sock_fd , buf , size, 0);

    if (write_size == -1) {
        throw std::runtime_error(std::strerror(errno));
    }

    return write_size;
}

// Send overload for MessageType
int Client::send(const MessageType &msgType) {
    uint8_t typeRaw = static_cast<uint8_t>(msgType);
    return send(&typeRaw, sizeof(typeRaw));
}

// Send overload for std::string
int Client::send(const std::string &msg) {
    uint32_t length = htonl(static_cast<uint32_t>(msg.size())); // Convert to network byte order
    send(&length, sizeof(length));                             // Send the length first
    return send(msg.data(), msg.size());                       // Send the actual string data
}

// Send overload for Card
int Client::send(const Card &card) {
    uint8_t color = static_cast<uint8_t>(card.getColor());
    uint8_t type = static_cast<uint8_t>(card.getType());
    uint8_t number = static_cast<uint8_t>(card.getNumber());

    std::array<uint8_t, 3> data = {color, type, number};
    return send(data.data(), data.size());
}