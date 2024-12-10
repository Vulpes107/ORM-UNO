#include "Player.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <optional>

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
            lowerInput.erase(lowerInput.find("uno"), 3); // Remove "uno" to avoid confusion
        }

        // Remove the command keyword and pass remaining tokens to parseCardData
        tokens.erase(tokens.begin());
        result.card = Card::parseCardData(tokens);
    } else if (tokens[0] == "draw") {
        result.type = CommandType::DRAW;
    } else if (tokens[0] == "skip") {
        result.type = CommandType::SKIP;
    } else {
        throw std::invalid_argument("Unknown command");
    }

    return result;
}

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

ParsedCommand Player::prompt() {
    /// TODO: uraditi tcp request prompt i receive i staviti dobijeno u string 
    std::string input;

    return parseInput(input);
}