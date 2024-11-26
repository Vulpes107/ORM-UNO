#include <stdio.h>
#include <string.h>    //strlen
#include <unistd.h>    //write
#include <sys/socket.h>
#include <cassert>

#include "config.hpp"
#include "Server.hpp"

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
}

Server::~Server() {
    if (close(socket_desc) < 0) {
        perror("Error closing socket");
    }
}

int Server::waitForConnection() {
    //accept connection from an incoming client
    sockaddr_in client;
    socklen_t addr_len = sizeof(sockaddr_in);

    int client_sock = accept(socket_desc, (sockaddr *)&client, &addr_len);
    if (client_sock < 0) {
        perror("accept failed");
    } else {
        puts("Connection accepted");
    }

    return client_sock;
}

int Server::receive(int client_sock, void *buf, const size_t size) {
    //Receive a message from client
    int read_size = recv(client_sock , buf , size , 0);

    if(read_size == 0) {
        puts("Client disconnected");
    } else if(read_size == -1) {
        perror("recv failed");
    } else {
        printf("Received: %d bytes!\n", read_size);
    }

    return read_size;
}

int Server::send(int client_sock, const void *buf, const size_t size) {
    const size_t write_size = ::send(client_sock , buf , size, 0);

    if (write_size != size) {
        puts("Send failed");
    }

    return write_size;
}