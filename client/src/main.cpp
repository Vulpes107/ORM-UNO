#include <iostream>
#include <string.h>
#include <Client.hpp>
#include <Player.hpp>
#include <csignal>

using namespace std;

int main() {
    string username;
    std::cout<<"Unesi username: ";
    std::getline(std::cin, username);

    try {
        Client client;
        client.connectToServer();

        Player player(username, client);

        while(true) {
            try {
                auto rez = player.prompt();
                std::cout<<static_cast<int>(rez.type)<<" | "<<rez.card->toString()<<" | "<<rez.saidUno<<std::endl;
            } catch(exception &e) {
                std::cerr<<"Input error: "<<e.what()<<std::endl;
            }
        }
    } catch(exception &e) {
        std::cerr<<"Server error: "<<e.what()<<std::endl;
    }
    

    // if (!client.connectToServer()) {
    //     cerr << "Connect failed" << endl;
    //     return 1;
    // }

    // cout << "Input username: " << endl;
    // cin >> user_name;

    // char buf[256];
    // try {
    //     client.send(user_name.c_str(), user_name.size());
    //     client.receive(buf, 256);
    //     cout << buf << endl;   
    // } catch(const std::exception& e) {
    //     cerr << e.what() << endl;
    // }
   
    return 0;
}