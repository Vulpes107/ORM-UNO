#ifndef GAME_HPP
#define GAME_HPP

#include <Server.hpp>
#include <Player/Player.hpp>
#include <Deck/Deck.hpp>

#define NUMBER_OF_CARDS 7

enum class Direction { CW, CCW };

class Game {
private:
    Server &server;
    Players players;
    unsigned currentPlayer;

    Deck discardPile;
    Deck drawPile;
    Direction direction;

    Player &getNextPlayer();

public:
    Game(Server &server, Players players);
    
    void placeTopCard();
    void dealCards();
    void playNext();
};

#endif