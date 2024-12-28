#include "Card.hpp"
#include <stdexcept>
#include <sstream>
#include <optional>
#include <unordered_map>
#include <algorithm>

// Constructor
Card::Card(Color cardColor, Type cardType, int cardNumber)
    : color(cardColor), type(cardType), number(cardNumber) {
    if (type == Type::NUMBER && (number < 0 || number > 9)) {
        throw std::invalid_argument("Invalid number for a NUMBER card (must be between 0 and 9).");
    }
    if (type != Type::NUMBER && number != -1) {
        throw std::invalid_argument("Only NUMBER cards can have a valid number.");
    }
}

Card::Card() : color(Color::WILD), type(Type::WILD), number(-1) {}

// Getters
Color Card::getColor() const {
    return color;
}

Type Card::getType() const {
    return type;
}

int Card::getNumber() const {
    return number;
}

void Card::setWildColor(Color color) {
    if (type == Type::WILD || type == Type::WILD_DRAW_FOUR) {
        this->color = color;
    }
}

// Determines if the card can be placed on the topCard
bool Card::canBePlaced(Card &topCard) {
    // Check if card is a Wild card
    if (type == Type::WILD || type == Type::WILD_DRAW_FOUR) {
        return true;
    }

    // Check if both cards have the 'Draw_two' type
    if (type == Type::DRAW_TWO && topCard.getType() == Type::DRAW_TWO) {
        return true;
    }

    // Check if both cards have the 'Reverse' type
    if (type == Type::REVERSE && topCard.getType() == Type::REVERSE) {
        return true;
    }

    // Check if the colors match 
    if (color == topCard.color) {
        return true;
    }

    // Check if the numbers match
    if (number == topCard.number) {
        return true;
    }

    // If none of the conditions are met, the card cannot be placed
    return false;
}

// Utility to get card description
std::string Card::toString() const {
    std::ostringstream oss;

    // Add color
    switch (color) {
        case Color::RED:    oss << "Red "; break;
        case Color::GREEN:  oss << "Green "; break;
        case Color::BLUE:   oss << "Blue "; break;
        case Color::YELLOW: oss << "Yellow "; break;
        case Color::WILD:   oss << "Wild "; break;
    }

    // Add type and number
    switch (type) {
        case Type::NUMBER:
            oss << "Number " << this->getNumber();
            break;
        case Type::SKIP:
            oss << "Skip";
            break;
        case Type::REVERSE:
            oss << "Reverse";
            break;
        case Type::DRAW_TWO:
            oss << "Draw Two";
            break;
        case Type::WILD:
            oss << "Wild";
            break;
        case Type::WILD_DRAW_FOUR:
            oss << "Wild Draw Four";
            break;
    }

    return oss.str();
}

Card Card::parseCardData(std::vector<std::string> tokens) {
    // Extract card details (type and color)
    std::optional<Color> cardColor;
    std::optional<Type> cardType;
    std::optional<int> cardNumber;

    static const std::unordered_map<std::string, Color> colorMap = {
        {"red", Color::RED}, {"green", Color::GREEN}, {"blue", Color::BLUE}, {"yellow", Color::YELLOW}};
    static const std::unordered_map<std::string, Type> typeMap = {
        {"number", Type::NUMBER},   {"skip", Type::SKIP}, {"reverse", Type::REVERSE},
        {"drawtwo", Type::DRAW_TWO}, {"wild", Type::WILD}, {"wilddrawfour", Type::WILD_DRAW_FOUR}};

    for (const auto& word : tokens) {
        // Check for color
        if (colorMap.count(word)) {
            cardColor = colorMap.at(word);
        }
        // Check for type
        if (typeMap.count(word)) {
            cardType = typeMap.at(word);
        }
        // Check for number
        if (cardType == Type::NUMBER && std::all_of(word.begin(), word.end(), ::isdigit)) {
            cardNumber = std::stoi(word);
        }
    }

    // Validate that all required fields are set
    if ((cardType == Type::WILD || cardType == Type::WILD_DRAW_FOUR) && !cardColor) {
        throw std::invalid_argument("Color for the next card is missing or invalid.");
    }
    if (!cardColor) {
        throw std::invalid_argument("Card color is missing or invalid.");
    }
    if (!cardType) {
        throw std::invalid_argument("Card type is missing or invalid.");
    }
    if (cardType == Type::NUMBER && !cardNumber) {
        throw std::invalid_argument("Card number is missing for a number card.");
    }

    // If valid, return the card
    return Card(cardColor.value(), cardType.value(), cardNumber.value_or(-1));
}