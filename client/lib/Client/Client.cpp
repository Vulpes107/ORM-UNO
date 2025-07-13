#include <sys/socket.h> //socket
#include <fcntl.h>      //for open
#include <unistd.h>     //for close
#include <cassert>      //for assert
#include <stdexcept>
#include <iostream>
#include <cerrno>
#include <clocale>
#include <cstring>

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
    if (!disconnect()) {
        std::cerr << "[CLIENT ERROR] Error closing socket" << std::endl;
    }
}

void Client::connectToServer() {
    if (connect(sock_fd, (struct sockaddr*)&server, sizeof(server)) != -1) {
        char *ip = inet_ntoa(server.sin_addr);
        uint16_t port = ntohs(server.sin_port);
        std::cout << "[CLIENT] Connected to server at " << ip << ":" << port << std::endl;
    } else {
        throw std::runtime_error("Unable to connect to server");
    }
}

bool Client::disconnect() {
    return close(sock_fd) >= 0;
}

int Client::receive(void *buf, const size_t size) {
    //Receive a message from server
    int read_size = recv(sock_fd , buf , size , 0);

    if(read_size == 0) {
        throw std::runtime_error("Server disconnected");
    } else if(read_size == -1) {
        throw std::runtime_error(std::strerror(errno));
    } else {
        // std::cout << "[CLIENT] Received: " << read_size << " bytes" << std::endl;
    }

    return read_size;
}

// Receive overload for MessageType
int Client::receive(MessageType &msgType) {
    return receive(&msgType, sizeof(MessageType));
}

// Receive overload for std::string
int Client::receive(std::string &msg) {
    int length;
    receive(length);

    std::vector<char> buffer(length);
    receive(buffer.data(), length); // Receive string data
    msg.assign(buffer.begin(), buffer.end());
    return length;
}

// Receive overload for Card
int Client::receive(Card &card) {
    int length;
    receive(length);
    std::vector<uint8_t> buffer(length);
    receive(buffer.data(), length);
    card = Card::deserialize(buffer);
    return length;
}

int Client::receive(int &num) {
    int size = receive(&num, sizeof(num)); // Receive num
    num = ntohl(num);          // Convert to host byte order
    return size;
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
    return send(&msgType, sizeof(MessageType));
}

// Send overload for std::string
int Client::send(const std::string &msg) {
    send(msg.size());                               // Send length
    return send(msg.data(), msg.size());            // Send string data
}

// Send overload for Card
int Client::send(const Card &card) {
    std::vector<uint8_t> data = card.serialize();
    send(static_cast<int>(data.size())); // send length first
    return send(data.data(), data.size());
}

int Client::send(const int &num) {
    int net_num = htonl(num); // Convert to network byte order
    return send(&net_num, sizeof(net_num));
}