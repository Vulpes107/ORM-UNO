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
        result.type = CommandType::PLACE;

        // Check if "uno" is present
        if (lowerInput.find("uno") != std::string::npos) {
            result.saidUno = true;
            lowerInput.erase(lowerInput.find("uno"), 3); // Remove "uno"
        }

        // Remove the command keyword and pass remaining tokens to parseCardData
        tokens.erase(tokens.begin());
        result.card = Card::parseCardData(tokens);
    } else if (tokens[0] == "draw") {
        result.type = CommandType::DRAW;
    } else if (tokens[0] == "skip") {
        result.type = CommandType::SKIP;
    } else if (tokens[0] == "top") {
        result.type = CommandType::TOP;
    } else if (tokens[0] == "deck") {
        result.type = CommandType::DECK;
    } else {
        throw std::invalid_argument("Unknown command");
    }

    return result;
}

ParsedCommand Player::prompt() {
    while (true) {
        try {
            std::string input;
            std::cout<<"Input: ";
            std::getline(std::cin, input);

            return parseInput(input);
        } catch(std::exception &e) {
            std::cerr<<"Input error: "<<e.what()<<std::endl;
        }
    }
}

void Player::setHandDeck() {
    MessageType type;
    client.receive(type);
    if(type == MessageType::HAND_DECK) {
        this->handDeck = {};    // empty
        addCards(NUMBER_OF_CARDS_TO_DEAL);
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

bool Player::findCard(Card &card) {
    std::unordered_set<Color> colorSet({Color::BLUE, Color::GREEN, Color::RED, Color::YELLOW}); // Only for wild card
    std::vector<Card>::iterator it;

    if (handDeck.empty()) {
        throw std::runtime_error("handDeck is empty");
    }

        // std::cout<<"Deck is empty."<<std::endl;
        // return false;
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

    // Removing card from the hand deck
    handDeck.erase(it);
    return true;  
}

void Player::receiveServerCommand() {
    MessageType type;
    client.receive(type);
    switch (type)
    {
    case MessageType::TURN_TOKEN: {
        nextMove();
        break;
    }

    case MessageType::ECHO_MSG: {
        std::string str;
        client.receive(str);
        std::cout << "[ECHO] " << str << std::endl;
        break;
    }

    case MessageType::TOP_CARD: {
        client.receive(topCard);
        std::cout << "[TOP] " << topCard.toString() << std::endl;
        break;
    }

    case MessageType::DRAW: {
        int drawNum;
        client.receive(drawNum);
        addCards(drawNum);
        std::cout << "[DRAW] " << drawNum << std::endl;
        printHandDeck();
        break;
    }
    
    default:
        break;
    }
}

void Player::nextMove() {
    bool drawnOnce = false;
    bool turnToken = true;

    // Checking the top card of the discard pile
    std::cout<<"Top card: "<<topCard.toString()<<std::endl;

    while (turnToken) {
        ParsedCommand command = prompt();
        switch (command.type) {
        case CommandType::PLACE: {
            if (findCard(command.card.value())) {

                client.send(MessageType::PLACE);
                client.send(command.card.value());
                turnToken = false;

                std::cout<<static_cast<int>(command.type)<<" | "<<command.card->toString()<<" | "<<command.saidUno<< " | "<<command.card->getNumber() <<std::endl;
            }
            break;
        }

        case CommandType::DRAW: {
            if (!drawnOnce) {
                Card card;
                client.send(MessageType::DRAW);
                client.receive(card);
                handDeck.push_back(card);
                std::cout<<"Drawn card: "<<card.toString()<<std::endl;
                drawnOnce = true;
            } else {
                std::cout<<"Can't draw card more than once."<<std::endl;
            }
            break;
        }

        case CommandType::SKIP: {
            turnToken = false;
            break;
        }

        case CommandType::TOP: {
            std::cout<<"Top card: "<<topCard.toString()<<std::endl;
            break;
        }

        case CommandType::DECK: {
            printHandDeck();
            break;
        }
        }
    }

    client.send(MessageType::TURN_TOKEN);
    std::cout<<"new turn"<<std::endl;
}
