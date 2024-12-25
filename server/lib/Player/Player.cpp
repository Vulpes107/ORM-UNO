#include "Player.hpp"


Player::Player(Server &server, int socket, const std::string username) : server(server), socket(socket), username(username) {}

int Player::getSocket() const {
    return socket;
}

std::string Player::getName() const {
    return username;
}

void Player::setHandDeck(std::vector<Card> handDeck) {
    for(int i = 0; i < NUMBER_OF_CARDS_TO_DEAL; i++) {
        server.send(socket, handDeck[i]);
    }
}