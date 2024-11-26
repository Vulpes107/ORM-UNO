#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket

#include <fcntl.h>      //for open
#include <unistd.h>     //for close

#include <cassert>    //for assert

#include <TCP/config.hpp>
#include "Client.hpp"

Client::Client() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock_fd != -1);  // Failed to create a socket
    
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server.sin_port = htons(DEFAULT_PORT);
}

Client::~Client() {
    if (close(sock_fd) < 0) {
        perror("Error closing socket.");
    }
}

int Client::connectToServer() {
    return connect(sock_fd, (struct sockaddr*)&server, sizeof(server));
}

int Client::receive(void *buf, const size_t size) {
    //Receive a message from server
    int read_size = recv(sock_fd , buf , size , 0);

    if(read_size == 0) {
        puts("Client disconnected");
    } else if(read_size == -1) {
        perror("recv failed");
    } else {
        printf("Received: %d bytes!\n", read_size);
    }

    return read_size;
}

int Client::send(const void *buf, const size_t size) {
    const size_t write_size = ::send(sock_fd , buf , size, 0);

    if (write_size != size) {
        puts("Send failed");
    }

    return write_size;
}