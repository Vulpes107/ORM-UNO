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

    int opt = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    assert(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) >= 0);

    //Listen
    listen(socket_desc , 3);
    std::cout << "[SERVER] Listening on port " << DEFAULT_PORT << std::endl;
}

Server::~Server() {
    if (close(socket_desc) < 0) {
        std::cerr << "[SERVER ERROR] Error closing socket" << std::endl;
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
    std::cout << "[SERVER] Connection accepted from " << client_ip << ":" << client_port << std::endl;

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
        // std::cout << "Received: " << read_size << " bytes" << std::endl;
    }

    return read_size;
}

// Receive overload for MessageType
int Server::receive(int client_sock, MessageType &msgType) {
    return receive(client_sock, &msgType, sizeof(MessageType));
}

// Receive overload for std::string
int Server::receive(int client_sock, std::string &msg) {
    int length;
    receive(client_sock, length);

    std::vector<char> buffer(length);
    receive(client_sock, buffer.data(), length);  // Receive string data
    msg.assign(buffer.begin(), buffer.end());
    return length;
}

// Receive overload for Card
int Server::receive(int client_sock, Card &card) {
    int length;
    receive(client_sock, length);
    std::vector<uint8_t> buffer(length);
    receive(client_sock, buffer.data(), length);
    card = Card::deserialize(buffer);
    return length;
}

int Server::receive(int client_sock, int &num) {
    int size = receive(client_sock, &num, sizeof(num)); // Receive num
    num = ntohl(num);                        // Convert to host byte order
    return size;
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
    return send(client_sock, &msgType, sizeof(MessageType));
}

// Send overload for std::string
int Server::send(int client_sock, const std::string &msg) {
    send(client_sock, msg.size());                             // Send length
    return send(client_sock, msg.data(), msg.size());          // Send string data
}

// Send overload for Card
int Server::send(int client_sock, const Card &card) {
    std::vector<uint8_t> data = card.serialize();
    send(client_sock, static_cast<int>(data.size())); // send length first
    return send(client_sock, data.data(), data.size());
}

int Server::send(int client_sock, const int &num) {
    int net_num = htonl(num); // Convert to network byte order
    return send(client_sock, &net_num, sizeof(net_num));
}