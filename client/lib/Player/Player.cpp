#include "Player.hpp"
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <unordered_set>

Player::Player(const std::string username, Client &client) : username(username), client(client) {
    endGame = false;
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
            std::cout << "[INPUT] Enter your command (place/draw/skip/top/deck): ";
            std::getline(std::cin, input);

            return parseInput(input);
        } catch(std::exception &e) {
            std::cerr << "[INPUT ERROR] " << e.what() << std::endl;
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

bool Player::findCard(Card &card) {
    std::unordered_set<Color> colorSet({Color::BLUE, Color::GREEN, Color::RED, Color::YELLOW}); // Only for wild card
    std::vector<Card>::iterator it;

    if (handDeck.empty()) {
        throw std::runtime_error("Your hand deck is empty.");
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
        std::cout << "[INFO] Card does not exist in your deck." << std::endl;
        return false;
    } else if (!it->canBePlaced(topCard)) {
        std::cout << "[INFO] Card can't be placed on the current top card." << std::endl;
        return false;
    }

    // Removing card from the hand deck
    handDeck.erase(it);
    return true;  
}

void Player::receiveServerCommand() {
    try {
        MessageType type;
        client.receive(type);
        switch (type)
        {
        case MessageType::TURN_TOKEN: {
            std::cout << "\n[TURN] It's your turn!" << std::endl;
            nextMove();
            break;
        }

        case MessageType::ECHO_MSG: {
            std::string str;
            client.receive(str);
            std::cout << "[SERVER] " << str << std::endl;
            break;
        }

        case MessageType::TOP_CARD: {
            client.receive(topCard);
            std::cout << "[TOP CARD] " << topCard.toString() << std::endl;
            break;
        }

        case MessageType::DRAW: {
            int drawNum;
            client.receive(drawNum);
            addCards(drawNum);
            std::cout << "[DRAW] You drew " << drawNum << " card(s)." << std::endl;
            printHandDeck();
            break;
        }

        case MessageType::END_GAME: {
            std::string info;
            client.receive(info);
            std::cout << "[GAME OVER] " << info << std::endl;
            endGame = true;
            break;
        }
        
        default:
            break;
        }
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT ERROR] Lost connection to server: " << e.what() << std::endl;
        std::cerr << "[CLIENT] The server has shut down or disconnected. Exiting game." << std::endl;
        endGame = true;
    }
}

void Player::nextMove() {
    bool drawnOnce = false;
    bool turnToken = true;

    while (turnToken) {
        ParsedCommand command = prompt();
        switch (command.type) {
        case CommandType::PLACE: {
            if (findCard(command.card.value())) {
                if (handDeck.size() == 1 && !command.saidUno) { // player didn't say uno
                    std::cout << "[WARNING] You didn't say UNO! Drawing 2 penalty cards." << std::endl;
                    for (int i = 0; i < 2; i++) {
                        Card card;
                        client.send(MessageType::DRAW);
                        client.receive(card);
                        handDeck.push_back(card);
                        std::cout << "[PENALTY DRAW] " << card.toString() << std::endl;
                    }
                }
                client.send(MessageType::PLACE);
                client.send(command.card.value());
                turnToken = false;
            }
            break;
        }

        case CommandType::DRAW: {
            if (!drawnOnce) {
                Card card;
                client.send(MessageType::DRAW);
                client.receive(card);
                handDeck.push_back(card);
                std::cout << "[DRAW] You drew: " << card.toString() << std::endl;
                drawnOnce = true;
            } else {
                std::cout << "[INFO] You can't draw more than one card per turn." << std::endl;
            }
            break;
        }

        case CommandType::SKIP: {
            if (!drawnOnce) {
                std::cout << "[INFO] Skipping is not allowed unless a card has been drawn first." << std::endl;
            } else {
                std::cout << "[TURN] You skipped your turn." << std::endl;
                turnToken = false;
            }
            break;
        }

        case CommandType::TOP: {
            std::cout << "[TOP CARD] " << topCard.toString() << std::endl;
            break;
        }

        case CommandType::DECK: {
            printHandDeck();
            break;
        }
        }
    }

    client.send(MessageType::TURN_TOKEN);
}

bool Player::getEndGame() {
    return endGame;
}
