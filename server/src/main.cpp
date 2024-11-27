#include <iostream>
#include <Server.hpp>

using namespace std;

int main() {
    Server server;

    char buf[256];
    int client_sock = server.waitForConnection();

    try {
        int received_size = server.receive(client_sock, buf, 256);
        server.send(client_sock, buf, received_size);
        cout<<buf<<endl;
    } catch(const std::exception& e) {
      cerr << e.what() << endl;
    }

}