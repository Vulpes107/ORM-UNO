#include <iostream>
#include <Server.hpp>


int main() {
    using namespace std;
    Server server;
    char buf[256];
    int client_sock = server.waitForConnection();
    int received_size = server.receive(client_sock, buf, 256);
    server.send(client_sock, buf, received_size);
    cout<<buf<<endl;

}