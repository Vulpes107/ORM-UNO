#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>  //inet_addr
#include <Messages/MessageType.hpp>
#include <Card/Card.hpp>

class Client {
private:
    int sock_fd;
    sockaddr_in server;

public:
    Client();
    ~Client();

    void connectToServer();
    bool disconnect();

    int receive(void *buf, const size_t size);
    int receive(MessageType &msgType);
    int receive(std::string &msg);
    int receive(Card &card);

    int send(const void *buf, const size_t size);
    int send(const MessageType &msgType);
    int send(const std::string &msg);
    int send(const Card &card);
};

#endif // CLIENT_HPP