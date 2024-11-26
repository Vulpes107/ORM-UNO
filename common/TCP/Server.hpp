#include<arpa/inet.h> //inet_addr

#include <list>

class Server {
private:
    int socket_desc;
    sockaddr_in server;

public:
    Server();
    ~Server();

    int waitForConnection();
    int receive(int client_sock, void *client_message, const size_t size);
    int send(int client_sock, const void *buf, const size_t size);
};