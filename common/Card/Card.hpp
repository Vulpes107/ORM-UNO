#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <vector>
#include <cstdint>

// Enumeration for UNO card colors
enum class Color : uint8_t {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WILD
};

// Enumeration for UNO card types
enum class Type : uint8_t {
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
    int8_t number; // Used only if type == NUMBER

public:
    // Constructor
    Card(Color cardColor, Type cardType, int cardNumber = -1);
    Card();

    // Getters
    Color getColor() const;
    Type getType() const;
    int getNumber() const;

    void setWildColor(Color color);
    bool canBePlaced(Card &topCard);

    // Utility to get card description
    std::string toString() const;

    // Serialization: converts Card to a byte vector
    std::vector<uint8_t> serialize() const;

    // Deserialization: fills Card from a byte vector
    static Card deserialize(const std::vector<uint8_t>& data);

    static Card parseCardData(std::vector<std::string> tokens);
};

#endif // CARD_HPP
