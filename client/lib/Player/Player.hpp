#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <optional>

#include <Card/Card.hpp>
#include <../server/lib/Deck/Deck.hpp>
#include <Client.hpp>

enum CommandType {
    PLACE,
    SKIP,
    TOP,
    DRAW,
    DECK,
};

struct ParsedCommand {
    CommandType type;
    std::optional<Card> card; // Only for PLACE commands
    bool saidUno = false;     // True if "UNO" was detected
};

class Player {
private:
    std::string username;
    std::vector<Card> handDeck;
    Client &client;
    Card topCard;

    ParsedCommand parseInput(const std::string& input);
    ParsedCommand prompt();
    bool findCard(Card &card);
    void addCards(int num);
    void nextMove();

public:
    Player(const std::string username, Client &client);
    // ~Player();

    void setHandDeck();
    void printHandDeck();
    void receiveServerCommand();
    std::vector<Card> getHandDeck() const { return handDeck; }
    Card getTopCard() const { return topCard; }
};

#endif // PLAYER_HPP