#include "Deck.hpp"

#include <vector>
#include <algorithm>
#include <random>

// Initializes a full UNO deck
void Deck::initializeDeck() {
    std::vector<Card> deck;

    // Add numbered cards (0-9)
    for (auto color : {Color::RED, Color::GREEN, Color::BLUE, Color::YELLOW}) {
        for (int i = 0; i <= 9; ++i) {
            deck.emplace_back(color, Type::NUMBER, i);
            if (i != 0) { // Two of each number card (except 0)
                deck.emplace_back(color, Type::NUMBER, i);
            }
        }

        // Add special action cards (Skip, Reverse, Draw Two)
        for (int i = 0; i < 2; ++i) {
            deck.emplace_back(color, Type::SKIP);
            deck.emplace_back(color, Type::REVERSE);
            deck.emplace_back(color, Type::DRAW_TWO);
        }
    }

    // Add Wild and Wild Draw Four cards
    for (int i = 0; i < 4; ++i) {
        deck.emplace_back(Color::WILD, Type::WILD);
        deck.emplace_back(Color::WILD, Type::WILD_DRAW_FOUR);
    }

    // Shuffle the deck before pushing into the stack
    std::shuffle(deck.begin(), deck.end(), std::random_device());
    for (const auto& card : deck) {
        cards.push(card);
    }
}

// Resets the deck to its empty state
void Deck::resetEmpty() {
    while (!cards.empty()) {
        cards.pop();
    }
}

// Resets the deck to its full state
void Deck::resetFull() {
    resetEmpty();
    initializeDeck();
}

// Shuffles the deck
void Deck::shuffle() {
    std::vector<Card> deck;

    // Move cards into a vector
    while (!cards.empty()) {
        deck.push_back(cards.top());
        cards.pop();
    }

    // Shuffle the vector
    std::shuffle(deck.begin(), deck.end(), std::random_device());

    // Push cards back into the stack
    for (const auto& card : deck) {
        cards.push(card);
    }
}

// Returns the top card without removing it
const Card& Deck::getTopCard() const {
    if (cards.empty()) {
        throw std::runtime_error("Deck is empty. Cannot get top card.");
    }
    return cards.top();
}

// Removes and returns the top card
Card Deck::takeTopCard() {
    if (cards.empty()) {
        throw std::runtime_error("Deck is empty. Cannot take top card.");
    }
    Card topCard = cards.top();
    cards.pop();
    return topCard;
}

// Places a card on top of the deck
void Deck::placeCard(const Card& card) {
    cards.push(card);
}

// Checks if the deck is empty
bool Deck::isEmpty() const {
    return cards.empty();
}

void Deck::regenerateDrawPile(Deck& drawPile, Deck& discardPile) {
    if (discardPile.isEmpty()) {
        throw std::runtime_error("Discard pile is empty. Cannot regenerate draw pile.");
    }

    // Get the top card of the discard pile
    Card topCard = discardPile.takeTopCard();

    // Move all other cards from discard pile to a temporary vector
    std::vector<Card> newDrawPile;
    while (!discardPile.isEmpty()) {
        newDrawPile.push_back(discardPile.takeTopCard());
    }

    // Shuffle the cards
    std::shuffle(newDrawPile.begin(), newDrawPile.end(), std::random_device());

    // Add shuffled cards to the draw pile
    for (const auto& card : newDrawPile) {
        drawPile.placeCard(card);
    }

    // Place the top card back on the discard pile
    discardPile.placeCard(topCard);
}
