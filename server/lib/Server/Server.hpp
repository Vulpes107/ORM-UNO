#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h> //inet_addr
#include <Messages/MessageType.hpp>
#include <Card/Card.hpp>

class Server {
private:
    int socket_desc;
    sockaddr_in server;

public:
    Server();
    ~Server();

    int waitForConnection();
    
    int receive(int client_sock, void *buf, size_t size);
    int receive(int client_sock, MessageType &msgType);
    int receive(int client_sock, std::string &msg);
    int receive(int client_sock, Card &card);

    int send(int client_sock, const void *buf, size_t size);
    int send(int client_sock, const MessageType &msgType);
    int send(int client_sock, const std::string &msg);
    int send(int client_sock, const Card &card);
};

#endif