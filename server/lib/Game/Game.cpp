#include "Game.hpp"
#include <iostream>
#include <cstdlib>

Game::Game(Server &server, Players players) : server(server), players(players), direction(Direction::CW) {
    currentPlayer = rand() % players.size();
    discardPile.resetEmpty();
    drawPile.resetFull();
    prevTopCard = nullptr;
}

Game::~Game() {
    prevTopCard = nullptr;
}

void Game::dealCards() {
    for(auto &player : players) {
        std::vector<Card> hand;

        for(int i = 0; i < NUMBER_OF_CARDS_TO_DEAL; i++) {
            hand.push_back(drawCard());
        }

        player.setHandDeck(hand);
    }
}

void Game::placeTopCard() {
    while(drawPile.getTopCard().getType() != Type::NUMBER) {
        drawPile.shuffle();
    }

    discardPile.placeCard(drawCard());
    const Card &topCard = discardPile.getTopCard();
    prevTopCard = &topCard;
}

Player &Game::getNextPlayer() {
    if(direction == Direction::CW) {
        currentPlayer = (currentPlayer + 1) % players.size();
    } else {
        currentPlayer = (currentPlayer - 1) % players.size();
    }

    return players.at(currentPlayer);
}

void Game::broadcastTopCard() {
    Card topCard = discardPile.getTopCard();
    
    for(Player &player : players) {
        server.send(player.getSocket(), MessageType::TOP_CARD);
        server.send(player.getSocket(), topCard);
    }
}

void Game::topCardCommand() {
    const Card &topCard = discardPile.getTopCard();

    if (&topCard != prevTopCard) {   
        prevTopCard = &topCard;
        std::vector<Card> drawnCards;
        switch (topCard.getType()) {
        case Type::DRAW_TWO: {
            Player &player = getNextPlayer();
            for (int i = 0; i < 2; i++) {
                drawnCards.push_back(drawCard());
            }
            server.send(player.getSocket(), MessageType::DRAW);
            server.send(player.getSocket(), 2);
            player.addCards(drawnCards);
            break;
        }

        case Type::WILD_DRAW_FOUR: {
            Player &player = getNextPlayer();
            for (int i = 0; i < 4; i++) {
                drawnCards.push_back(drawCard());
            }
            server.send(player.getSocket(), MessageType::DRAW);
            server.send(player.getSocket(), 4);
            player.addCards(drawnCards);
            break;
        }

        case Type::REVERSE: {
            direction = (direction == Direction::CW) ? Direction::CCW : Direction::CW;
            if (players.size() == 2) {
                getNextPlayer();    // reverse acts as skip in two player situation
            }
            break;
        }

        case Type::SKIP: {
            getNextPlayer();
            break;
        }

        default: {
            break;
        }
        }
    }
}

Card Game::drawCard() {
    if(drawPile.isEmpty()) {
        Deck::regenerateDrawPile(drawPile, discardPile);
    }
    return drawPile.takeTopCard();
}

void Game::playNext() {
    Player &player = getNextPlayer();
    std::cout << "[PLAYER] " << player.getName() << std::endl;
    server.send(player.getSocket(), MessageType::TURN_TOKEN);   // tell him that it is his turn
    bool turnToken = false;

    while (!turnToken) {
        MessageType type;
        server.receive(player.getSocket(), type);

        switch (type) {

        case MessageType::PLACE: {
            Card card;
            server.receive(player.getSocket(), card);
            if(player.findCard(card)) {
                discardPile.placeCard(card);
            }
            break;
        }

        case MessageType::DRAW: {
            player.addCards({drawCard()});
            break;
        }

        case MessageType::TURN_TOKEN: {
            turnToken = true;
            break;
        }

        default: {
            std::cerr<<"Unknown packet order";
            break;
        }
        }
    }
}

bool Game::endGame() {
    bool endGame = false;
    for (unsigned i = 0; i < players.size(); i++) {
        if (players[i].endGame()) {
            endGame = true;
            break;
        }
    }
    if (endGame) {
        unsigned numPlayers = players.size();
        for (unsigned i = 0; i < numPlayers; i++) {
            if (players[i].endGame()) {
                server.send(players[i].getSocket(), MessageType::END_GAME);
                server.send(players[i].getSocket(), "Congratulations, you won!");   // tell him that is end of game
            } else {
                server.send(players[i].getSocket(), MessageType::END_GAME);
                server.send(players[i].getSocket(), "Sorry, you lost! Better luck next time!");  // tell him that is end of game
            }
        }
        return true;
    } else {
        return false;
    }
}