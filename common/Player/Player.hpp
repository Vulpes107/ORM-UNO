#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

#include "Card/Card.hpp"

class Player {
private:
    std::string username;
    std::vector<Card> hand_deck;
public:
    Player(const std::string user_name);
    
    
};

#endif // PLAYER_HPP