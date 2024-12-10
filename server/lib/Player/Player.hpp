#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

#include <Card/Card.hpp>
#include <Deck/Deck.hpp>

enum class CommandType {
    PLACE,
    DRAW,
    SKIP
};

struct ParsedCommand {
    CommandType type;
    std::optional<Card> card; // Only for PLACE commands
    bool saidUno = false;     // True if "UNO" was detected
};

class Player {
private:
    std::string username;
    int socket;
    std::vector<Card> handDeck;

    ParsedCommand parseInput(const std::string& input);

public:
    Player(int socket, const std::string username);

    int getSocket() const;
    std::string getName() const;
    
    void setHandDeck(std::vector<Card> handDeck);
    ParsedCommand prompt();
};

typedef std::vector<Player> Players;

#endif // PLAYER_HPP