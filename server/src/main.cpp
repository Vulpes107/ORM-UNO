#include <iostream>
#include <Server.hpp>
#include <Player/Player.hpp>
#include <Game/Game.hpp>

using namespace std;

int main() {
    cout << "==============================" << endl;
    cout << "   UNO Server Starting Up...  " << endl;
    cout << "==============================" << endl;

    Server server;
    Players players;    

    for(int i = 0; i < 2; i++) {
        cout << "[SERVER] Waiting for player " << (i+1) << " to connect..." << endl;
        int socket = server.waitForConnection();
        std::string username;
        server.receive(socket, username);
        cout << "[SERVER] Player " << (i+1) << " connected as '" << username << "'" << endl;
        players.emplace_back(server, socket, username);
    }

    cout << "[SERVER] All players connected. Starting game..." << endl;

    Game game(server, players);
    game.placeTopCard();
    game.dealCards();
    game.broadcastTopCard();
    try {
        while (!game.endGame()) {
            cout << "--------------------------------" << endl;
            cout << "[SERVER] Next turn..." << endl;
            game.playNext();
            game.broadcastTopCard();
            game.topCardCommand();
        }
        cout << "[SERVER] Game ended." << endl;
    } catch (std::exception &e) {
        std::cerr << "[SERVER ERROR] " << e.what() << std::endl;
        // Inform all players that the server is shutting down
        for (auto& player : players) {
            try {
                server.send(player.getSocket(), MessageType::END_GAME);
                server.send(player.getSocket(), "Server has shut down unexpectedly.");
            } catch (...) {
                // Ignore errors if client already disconnected
            }
        }
    }
}