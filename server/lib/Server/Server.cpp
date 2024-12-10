#include <unistd.h>
#include <sys/socket.h>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cerrno>
#include <clocale>
#include <cstring>
#include <array>

#include "Server.hpp"
#include <TCP/config.hpp>

Server::Server() {
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    assert(socket_desc != -1);

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    assert(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) >= 0);

    //Listen
    listen(socket_desc , 3);
    std::cout << "Server has started" << std::endl;
}

Server::~Server() {
    if (close(socket_desc) < 0) {
        std::cerr << "Error closing socket" << std::endl;
    }
}

int Server::waitForConnection() {
    //accept connection from an incoming client
    sockaddr_in client;
    socklen_t addr_len = sizeof(sockaddr_in);

    int client_sock = accept(socket_desc, (sockaddr *)&client, &addr_len);
    if (client_sock < 0) {
        throw std::runtime_error("Accept failed");
    }

    char *client_ip = inet_ntoa(client.sin_addr); // Convert IP to string
    uint16_t client_port = ntohs(client.sin_port); // Convert port to host byte order
    std::cout << "Connection accepted from " << client_ip << ":" << client_port << std::endl;

    return client_sock;
}

int Server::receive(int client_sock, void *buf, const size_t size) {
    //Receive a message from client
    int read_size = recv(client_sock , buf , size , 0);

    if(read_size == 0) {
        throw std::runtime_error("Client disconnected");
    } else if(read_size == -1) {
        throw std::runtime_error(std::strerror(errno));
    } else {
        std::cout << "Received: " << read_size << " bytes" << std::endl;
    }

    return read_size;
}

// Receive overload for MessageType
int Server::receive(int client_sock, MessageType &msgType) {
    uint8_t typeRaw;
    int bytesRead = receive(client_sock, &typeRaw, sizeof(typeRaw));
    msgType = static_cast<MessageType>(typeRaw);
    return bytesRead;
}

// Receive overload for std::string
int Server::receive(int client_sock, std::string &msg) {
    uint32_t length;
    receive(client_sock, &length, sizeof(length)); // Receive length
    length = ntohl(length);                        // Convert to host byte order

    std::vector<char> buffer(length);
    receive(client_sock, buffer.data(), length);  // Receive string data
    msg.assign(buffer.begin(), buffer.end());
    return static_cast<int>(length);
}

// Receive overload for Card
int Server::receive(int client_sock, Card &card) {
    std::array<uint8_t, 3> data;
    receive(client_sock, data.data(), data.size());

    Color color = static_cast<Color>(data[0]);
    Type type = static_cast<Type>(data[1]);
    int number = static_cast<int>(data[2]);

    card = Card(color, type, number);
    return static_cast<int>(data.size());
}

int Server::send(int client_sock, const void *buf, const size_t size) {
    //Send a message to client
    const int write_size = ::send(client_sock , buf , size, 0);

    if (write_size == -1) {
        throw std::runtime_error(std::strerror(errno));
    }

    return write_size;
}

// Send overload for MessageType
int Server::send(int client_sock, const MessageType &msgType) {
    uint8_t typeRaw = static_cast<uint8_t>(msgType);
    return send(client_sock, &typeRaw, sizeof(typeRaw));
}

// Send overload for std::string
int Server::send(int client_sock, const std::string &msg) {
    uint32_t length = htonl(static_cast<uint32_t>(msg.size())); // Convert to network byte order
    send(client_sock, &length, sizeof(length));                // Send length
    return send(client_sock, msg.data(), msg.size());          // Send string data
}

// Send overload for Card
int Server::send(int client_sock, const Card &card) {
    uint8_t color = static_cast<uint8_t>(card.getColor());
    uint8_t type = static_cast<uint8_t>(card.getType());
    uint8_t number = static_cast<uint8_t>(card.getNumber());

    std::array<uint8_t, 3> data = {color, type, number};
    return send(client_sock, data.data(), data.size());
}