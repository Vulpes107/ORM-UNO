#include <iostream>
#include <Server.hpp>

using namespace std;

int main() {
    Server server;

    char client_username[256];

    try {
      int client_sock = server.waitForConnection();
      int received_size = server.receive(client_sock, client_username, 256);
      server.send(client_sock, client_username, received_size);
      cout<<client_username<<endl;
    } catch(const std::exception& e) {
      cerr << e.what() << endl;
    }

}