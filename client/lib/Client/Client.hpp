#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>  //inet_addr

class Client {
private:
    int sock_fd;
    sockaddr_in server;

public:
    Client();
    ~Client();

    bool connectToServer();
    int receive(void *client_message, const size_t size);
    int send(const void *buf, const size_t size);
};

#endif // CLIENT_HPP