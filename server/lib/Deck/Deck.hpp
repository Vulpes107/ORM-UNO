#ifndef DECK_HPP
#define DECK_HPP

#include <Card/Card.hpp>
#include <stack>
#include <stdexcept>

#define NUMBER_OF_CARDS_TO_DEAL 7

class Deck {
private:
    std::stack<Card> cards; // Represents the deck of cards

    // Helper method to initialize a full UNO deck
    void initializeDeck();

public:
    void resetEmpty();
    void resetFull();
    void shuffle();
    const Card& getTopCard() const; // see the card on top
    Card takeTopCard(); //take the card from the top
    void placeCard(const Card& card);
    bool isEmpty() const;

    static void regenerateDrawPile(Deck &drawPile, Deck &discardPile);
};

#endif // DECK_HPP
