#include "Card.hpp"
#include <stdexcept>
#include <sstream>

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
            oss << "Number " << number;
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
