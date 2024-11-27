#include <sys/socket.h> //socket
#include <fcntl.h>      //for open
#include <unistd.h>     //for close
#include <cassert>    //for assert
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
    
    std::cout << "Socket created" << std::endl;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server.sin_port = htons(DEFAULT_PORT);
}

Client::~Client() {
    if (close(sock_fd) < 0) {
        std::cerr << "Error closing socket" << std::endl;
    }
}

bool Client::connectToServer() {
    return connect(sock_fd, (struct sockaddr*)&server, sizeof(server)) != -1;
}

int Client::receive(void *buf, const size_t size) {
    //Receive a message from client
    int read_size = recv(sock_fd , buf , size , 0);

    if(read_size == 0) {
        throw std::runtime_error("Client disconnected");
    } else if(read_size == -1) {
        throw std::runtime_error(std::strerror(errno));
    } else {
        std::cout << "Received: " << read_size << " bytes" << std::endl;
    }

    return read_size;
}

int Client::send(const void *buf, const size_t size) {
    //Send a message to client
    const int write_size = ::send(sock_fd , buf , size, 0);

    if (write_size == -1) {
        throw std::runtime_error(std::strerror(errno));
    }

    return write_size;
}