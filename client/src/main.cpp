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

        try {
            player.setHandDeck();
            std::cout<<"Received deck;"<<endl;
        } catch(exception &e) {
            std::cerr<<"Error: "<<e.what()<<std::endl;
            return 0;
        }

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
    
   
    return 0;
}