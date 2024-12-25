#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <optional>

#include <Card/Card.hpp>
#include <../server/lib/Deck/Deck.hpp>
#include <Client.hpp>

struct ParsedCommand {
    MessageType type;
    std::optional<Card> card; // Only for PLACE commands
    bool saidUno = false;     // True if "UNO" was detected
};

class Player {
private:
    std::string username;
    std::vector<Card> handDeck;
    Client &client;

    ParsedCommand parseInput(const std::string& input);

public:
    Player(const std::string username, Client &client);
    // ~Player();

    ParsedCommand prompt();

    void setHandDeck();
};

#endif // PLAYER_HPP