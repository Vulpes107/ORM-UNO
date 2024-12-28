#include "Game.hpp"
#include <iostream>
#include <cstdlib>

Game::Game(Server &server, Players players) : server(server), players(players), direction(Direction::CW) {
    currentPlayer = rand() % players.size()+1;
    skipPlayer = false;
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
        if (skipPlayer) {
            currentPlayer = (currentPlayer+2)%players.size();
            skipPlayer = false;
        } else {
            currentPlayer = (currentPlayer+1)%players.size();
        }
    } else {
        if (skipPlayer) {
            currentPlayer = (currentPlayer-2)%players.size();
            skipPlayer = false;
        } else {
            currentPlayer = (currentPlayer-1)%players.size();
        }
    }

    return players.at(currentPlayer);
}

void Game::topCardComand(Player &player) {
    Card topCard = discardPile.getTopCard();
    std::vector<Card> drawnCards;
    switch (topCard.getType()) {
        case Type::DRAW_TWO: {
            for (int i = 0; i < 2; i++) {
                drawnCards.push_back(drawPile.takeTopCard());
            }
            player.setHandDeck(drawnCards);
            break;
        }

        case Type::WILD_DRAW_FOUR: {
            for (int i = 0; i < 4; i++) {
                drawnCards.push_back(drawPile.takeTopCard());
            }
            player.setHandDeck(drawnCards);
            skipPlayer = true;
            break;
        }

        case Type::REVERSE: {
            direction = (direction == Direction::CW) ? Direction::CCW : Direction::CW;
            break;
        }

        case Type::SKIP: {
            skipPlayer = true;
            break;
        }

        default: {
            break;
        }
    }
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
            topCardComand(player);
            break;
        }

        case MessageType::PLACE: {
            Card card;
            server.receive(player.getSocket(), card);
            if(player.findCard(card)) {
                discardPile.placeCard(card);
            }
            //TO DO: Ako vrati false sta da radi? -> fakticki je nemoguce vratiti false,
            //      jer je prethodno u clijentu provjereno da postoji data karta u deku,
            //      dakle ne treba nista?
            break;
        }

        case MessageType::DRAW: {
            int numberToDraw;
            Card card;
            std::vector<Card> drawnCards;
            server.receive(player.getSocket(), numberToDraw);
            for(int i = 0; i < numberToDraw; i++) {
                card = drawPile.takeTopCard();
                drawnCards.push_back(card);
                server.send(player.getSocket(), card);
            }
            player.setHandDeck(drawnCards);
            break;
        }

        case MessageType::SKIP: {
            turnToken = true;
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
    player.printHandDeck();
}