#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

#include <Card/Card.hpp>
#include <Deck/Deck.hpp>
#include <Server.hpp>

class Player {
private:
    Server &server;
    int socket;
    std::string username;

public:
    Player(Server &server, int socket, const std::string username);

    int getSocket() const;
    std::string getName() const;
    
    void setHandDeck(std::vector<Card> handDeck);
};

typedef std::vector<Player> Players;

#endif // PLAYER_HPP