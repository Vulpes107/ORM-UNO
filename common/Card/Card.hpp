#ifndef CARD_HPP
#define CARD_HPP

#include <string>

// Enumeration for UNO card colors
enum class Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WILD
};

// Enumeration for UNO card types
enum class Type {
    NUMBER,
    SKIP,
    REVERSE,
    DRAW_TWO,
    WILD,
    WILD_DRAW_FOUR
};

class Card {
private:
    Color color;
    Type type;
    int number; // Used only if type == NUMBER

public:
    // Constructor
    Card(Color cardColor, Type cardType, int cardNumber = -1);

    // Getters
    Color getColor() const;
    Type getType() const;
    int getNumber() const;

    void setWildColor(Color color);
    bool canBePlaced(Card &topCard);

    // Utility to get card description
    std::string toString() const;
};

#endif // CARD_HPP
