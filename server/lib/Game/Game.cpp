#include "Game.hpp"
#include <iostream>

Game::Game(Server &server, Players players) : server(server), players(players), direction(Direction::CW) {
    discardPile.resetEmpty();
    drawPile.resetFull();
}

void Game::dealCards() {
    for(auto &player : players) {
        std::vector<Card> hand;

        for(int i = 0; i < NUMBER_OF_CARDS; i++) {
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
    /// TODO: sve

    // if(drawPile.isEmpty()) {    // greska, drawPile < 5?
    //     Deck::regenerateDrawPile(drawPile, discardPile);
    // }

    Player &player = getNextPlayer();

    /*
    send to client what is requested from user:
    if he needs to draw 4 or 2 just tell him, draw and skip
    if he needs to place a card prompt him to place or draw
    */
    while(true) {
        try {
            ParsedCommand command = player.prompt();

            if(command.type == CommandType::DRAW) {
                // draw a card from drawPile and send it to client
                // Card card = drawPile.takeTopCard();
            } else if(command.type == CommandType::PLACE) {
                // place a card sent from client
                // end his turn (return false)

                // Card card = command.card.value();
                // discardPile.placeCard(card);
            } else {
                // client skipped, end his turn
            }
            
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error parsing input: " << e.what() << std::endl;
        }
    }

    
}