#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

#include <Card/Card.hpp>
#include <Server.hpp>

class Player {
private:
    Server &server;
    int socket;
    std::string username;
    std::vector<Card> handDeck;

public:
    Player(Server &server, int socket, const std::string username);

    int getSocket() const;
    std::string getName() const;
    std::vector<Card> getHandDeck() const;
    
    void setHandDeck(std::vector<Card> handDeck);
    bool findCard(Card card);
    void printHandDeck();
};

typedef std::vector<Player> Players;

#endif // PLAYER_HPP