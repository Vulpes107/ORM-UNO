#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

#include <Card/Card.hpp>
#include <Deck/Deck.hpp>

class Player {
private:
    std::string username;
    int socket;
    std::vector<Card> handDeck;


public:
    Player(int socket, const std::string username);

    int getSocket() const;
    std::string getName() const;
    
    void setHandDeck(std::vector<Card> handDeck);
};

typedef std::vector<Player> Players;

#endif // PLAYER_HPP