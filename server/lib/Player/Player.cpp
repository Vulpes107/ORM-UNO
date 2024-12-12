#include "Player.hpp"


Player::Player(int socket, const std::string username) : socket(socket), username(username) {}

int Player::getSocket() const {
    return socket;
}

std::string Player::getName() const {
    return username;
}

void Player::setHandDeck(std::vector<Card> handDeck) {
    // this->handDeck = handDeck;
    // send handDeck to client, no need to keep it
}

