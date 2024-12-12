#include "Game.hpp"
#include <iostream>

Game::Game(Server &server, Players players) : server(server), players(players), direction(Direction::CW) {
    discardPile.resetEmpty();
    drawPile.resetFull();
}

void Game::dealCards() {
    for(auto &player : players) {
        std::vector<Card> hand;

        for(int i = 0; i < NUMBER_OF_CARDS_TO_DEAL; i++) {
            hand.push_back(drawPile.takeTopCard());
        }
 
        player.setHandDeck(hand);
    }
}

void Game::placeTopCard() {
    while(drawPile.getTopCard().getType() != Type::NUMBER) {
        drawPile.shuffle();
    }

    discardPile.placeCard(drawPile.takeTopCard());
}

Player &Game::getNextPlayer() {
    if(direction == Direction::CW) {
        currentPlayer = (currentPlayer+1)%players.size();
    } else {
        currentPlayer = (currentPlayer-1)%players.size();
    }

    return players.at(currentPlayer);
}

void Game::playNext() {
    // if(drawPile.isEmpty()) {    // greska, drawPile < 5?
    //     Deck::regenerateDrawPile(drawPile, discardPile);
    // }

    Player &player = getNextPlayer();
    server.send(player.getSocket(), MessageType::TURN_TOKEN);   // tell him that it is his turn
    bool turnToken = false;

    while (!turnToken) {
        MessageType type;
        server.receive(player.getSocket(), type);

        switch (type) {
        case MessageType::TOP_CARD: {
            server.send(player.getSocket(), discardPile.getTopCard());
            break;
        }

        case MessageType::PLACE: {
            Card card;
            server.receive(player.getSocket(), card);
            discardPile.placeCard(card);
            break;
        }

        case MessageType::DRAW: {
            int numberToDraw;
            server.receive(player.getSocket(), numberToDraw);
            for(int i = 0; i < numberToDraw; i++) {
                server.send(player.getSocket(), drawPile.takeTopCard());
            }
            break;
        }

        case MessageType::SKIP: {
            break;
        }

        case MessageType::TURN_TOKEN: {
            turnToken = true;
            break;
        }

        default: {
            std::cerr<<"Unknow packet order";
            break;
        }
        }
    }
}