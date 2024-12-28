#include "Player.hpp"
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <unordered_set>

Player::Player(const std::string username, Client &client) : username(username), client(client) {
    client.send(username);
}

ParsedCommand Player::parseInput(const std::string& input) {
    ParsedCommand result;

    // Convert input to lowercase for case insensitivity
    std::string lowerInput = input;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // Tokenize the input
    std::istringstream stream(lowerInput);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        throw std::invalid_argument("Empty string");
    }

    // Parse command type
    if (tokens[0] == "place") {
        result.type = MessageType::PLACE;

        // Check if "uno" is present
        if (lowerInput.find("uno") != std::string::npos) {
            result.saidUno = true;
            lowerInput.erase(lowerInput.find("uno"), 3); // Remove "uno"
        }

        // Remove the command keyword and pass remaining tokens to parseCardData
        tokens.erase(tokens.begin());
        result.card = Card::parseCardData(tokens);
    } else if (tokens[0] == "draw") {
        result.type = MessageType::DRAW;
    } else if (tokens[0] == "skip") {
        result.type = MessageType::SKIP;
    } else {
        throw std::invalid_argument("Unknown command");
    }

    return result;
}

ParsedCommand Player::prompt() {
    std::string input;
    std::cout<<"Input: ";
    std::getline(std::cin, input);

    return parseInput(input);
}

bool Player::prompt(ParsedCommand &comand) {
    while (true) {
        try {
            comand = prompt();
            return true;
        } catch(std::exception &e) {
            std::cerr<<"Input error: "<<e.what()<<std::endl;
        }
    }
    return false;
}

void Player::setHandDeck() {
    Card card;
    for (int i = 0; i < NUMBER_OF_CARDS_TO_DEAL; i++) {
        client.receive(card);
        handDeck.push_back(card);
    }
}

void Player::addCards(int num){
    Card card;
    for (int i = 0; i < num; i++) {
        client.receive(card);
        handDeck.push_back(card);
    }
}

void Player::printHandDeck() {
    std::vector<Card>::iterator it;
    if (handDeck.empty()) {
        std::cout<<"Deck is empty"<<std::endl;
    } else {
        for(it = handDeck.begin(); it != handDeck.end(); it++) {
            std::cout<<(*it).toString()<<std::endl;
        }
    }
}

bool Player::findCard(Card &card, Card &topCard) {
    std::unordered_set<Color> colorSet({Color::BLUE, Color::GREEN, Color::RED, Color::YELLOW}); // Only for wild card
    std::vector<Card>::iterator it;

    if (handDeck.empty()) {
        std::cout<<"Deck is empty."<<std::endl;
        return false;
    } else {
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
            std::cout<<"Card does not exist in your deck."<<std::endl;
            return false;
        } else if (!it->canBePlaced(topCard)) {
            std::cout<<"Card can't be placed."<<std::endl;
            return false;
        }
    }

    // Removing card from the hand deck
    handDeck.erase(it);
    return true;  
}

void Player::topCardComand(Card &topCard) {
    switch (topCard.getType()) {
        case Type::DRAW_TWO: {
            addCards(2);
            break;
        }

        case Type::WILD_DRAW_FOUR: {
            addCards(4);
            break;
        }

        default: {
            break;
        }
        //TO DO: Kako obavijestiti igraca da je skipan ili da je obrnut smijer ako on tad ne moze izvuci gornju kartu

/*
        case Type::REVERSE: {
            direction = (direction == Direction::CW) ? Direction::CCW : Direction::CW;
            break;
        }

        case Type::SKIP: {
            skipPlayer = true;
            break;
        }

        default: {
            break;
        }*/
    }
}

void Player::nextMove() {
    MessageType type;
    client.receive(type);
    if (type == MessageType::TURN_TOKEN) {
        Card topCard;
        ParsedCommand comand;
        bool drawnOnce = false;
        bool turnToken = false;

        // Checking the top card of the discard pile
        client.send(MessageType::TOP_CARD);
        client.receive(topCard);
        std::cout<<"Top card: "<<topCard.toString()<<std::endl;
        topCardComand(topCard);

        while (true) {
            if (prompt(comand)) {
                switch (comand.type) {
                case MessageType::PLACE: {
                    if (findCard(comand.card.value(), topCard)) {

                        client.send(MessageType::PLACE);
                        client.send(comand.card.value());
                        turnToken = true;
                        client.send(MessageType::TURN_TOKEN);

                        std::cout<<static_cast<int>(comand.type)<<" | "<<comand.card->toString()<<" | "<<comand.saidUno<< " | "<<comand.card->getNumber() <<std::endl;
                    }
                    break;
                }    

                case MessageType::DRAW: {
                    if (!drawnOnce) {
                        Card card;
                        client.send(MessageType::DRAW);
                        client.send(1); 
                        client.receive(card);
                        handDeck.push_back(card);
                        std::cout<<"Drawn card: "<<card.toString()<<std::endl;
                    } else {
                        std::cout<<"Can't draw card more than once."<<std::endl;
                    }
                    break;
                }

                case MessageType::SKIP: {
                    turnToken = true;
                    client.send(MessageType::SKIP);
                    break;
                }

                default: {
                    std::cout<<"Unknown packet order"<<std::endl;
                    break;
                }
                }

                if (turnToken) {
                    break;
                }
            }
        }
        std::cout<<"new turn"<<std::endl;
    }
}
