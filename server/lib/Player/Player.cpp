#include "Player.hpp"
#include <unordered_set>
#include <iostream>


Player::Player(Server &server, int socket, const std::string username) : server(server), socket(socket), username(username) {}

int Player::getSocket() const {
    return socket;
}

std::string Player::getName() const {
    return username;
}

std::vector<Card> Player::getHandDeck() const {
    if (handDeck.empty()) {
        throw std::invalid_argument("Your hand deck is empty.");
    }
    return handDeck;
}

void Player::setHandDeck(std::vector<Card> handDeck) {
    this->handDeck = handDeck;
    server.send(getSocket(), MessageType::HAND_DECK);
    for (auto it = handDeck.begin(); it != handDeck.end(); it++) {
        server.send(socket, *it);
    }
}

void Player::addCards(std::vector<Card> cards) {
    for (auto it = cards.begin(); it != cards.end(); it++) {
        this->handDeck.push_back(*it);
        server.send(socket, *it);
    }
}

bool Player::findCard(Card card) {
    std::unordered_set<Color> colorSet({Color::BLUE, Color::GREEN, Color::RED, Color::YELLOW}); // Only for wild card
    std::vector<Card>::iterator it;

    if (handDeck.empty()) {
        return false;
    }
    
    for (it = handDeck.begin(); it != handDeck.end(); it++) {
        // Check for type
        if (it->getType() == card.getType()) {
            if (it->getType() == Type::WILD || it->getType() == Type::WILD_DRAW_FOUR) {
                // Check for next card color
                if (colorSet.count(card.getColor())) {
                    break;
                }
            } else {
                // Check for color
                if (it->getColor() == card.getColor()) {
                    if (it->getType() == card.getType()) {
                        // Check for number
                        if (it->getNumber() == card.getNumber()) {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }

    if (it == handDeck.end()) {
        return false;
    }

    // Removing card from the hand deck
    handDeck.erase(it);
    return true;  
}

// Only for functionality check
void Player::printHandDeck() {
    std::vector<Card>::iterator it;
    if (handDeck.empty()) {
        std::cout << "[INFO] Your hand deck is empty." << std::endl;
    } else {
        std::cout << "===== Your Hand Deck =====" << std::endl;
        int idx = 1;
        for(it = handDeck.begin(); it != handDeck.end(); it++, idx++) {
            std::cout << idx << ". " << (*it).toString() << std::endl;
        }
        std::cout << "==========================" << std::endl;
    }
}

bool Player::endGame() {
    return handDeck.empty();
}