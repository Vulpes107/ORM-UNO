#include "Player.hpp"
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

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

void Player::setHandDeck() {
    Card card;
    for(int i = 0; i < NUMBER_OF_CARDS_TO_DEAL; i++) {
        client.receive(card);
        handDeck.push_back(card);
    }
}