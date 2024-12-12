#include <iostream>
#include <Server.hpp>
#include <Player/Player.hpp>
#include <Game/Game.hpp>

using namespace std;

int main() {
    Server server;
    Players players;    

    for(int i = 0; i < 2; i++) {
        int socket = server.waitForConnection();
        std::string username;
        server.receive(socket, username);
        
        players.emplace_back(server, socket, username);
    }

    Game game(server, players);
    game.placeTopCard();
    try {
        game.dealCards();
        game.playNext();
    } catch (std::exception &e) {
        std::cerr<<"Server error: "<<e.what()<<std::endl;
    }
}