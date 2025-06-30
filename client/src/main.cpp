#include <iostream>
#include <string.h>
#include <Client.hpp>
#include <Player.hpp>
#include <unordered_map>
#include <csignal>
#include <TerminalUI/TerminalUI.hpp>
#include <thread>

// #include <TerminalUI/Assets/CardBox.hpp>
#include <TerminalUI/Assets/DeckBox.hpp>
#include <TerminalUI/Assets/TopCardBox.hpp>
#include <TerminalUI/Assets/PlayersBox.hpp>
#include <TerminalUI/Assets/EchoBox.hpp>

using namespace std;
using namespace UI;

Page gamePage;
DeckBox deckBox(1, 1);
TopCardBox topCardBox(DECK_BOX_WIDTH + 1, 1);
PlayersBox playersBox(DECK_BOX_WIDTH + 1, TOP_CARD_BOX_HEIGHT + 1);
EchoBox echoBox(1, DECK_BOX_HEIGHT + 1);

int main() {
    string username;
    while(true) {
        cout<<"Input username: ";
        getline(cin, username);
        if(username.empty()) {
            cout<<"String is empty."<<endl;
        } else {
            break;
        }
    }

    TerminalUI ui;
    // cout<<"w: "<<ui.getWidth()<<" h: "<<ui.getHeight()<<endl;
    ui.registerPage(&gamePage);
    gamePage.registerBox(&deckBox);
    gamePage.registerBox(&topCardBox);
    gamePage.registerBox(&playersBox);
    gamePage.registerBox(&echoBox);
    gamePage.focusBox(&deckBox);

    for(int i = 0; i < 10; i++) {
        echoBox.addMessage("hello " + std::to_string(i));
    }

    try {
        Client client;
        client.connectToServer();

        Player player(username, client);
        player.setHandDeck();
        player.receiveServerCommand();
        
        while(true) {
            // auto deck = player.getHandDeck();
            deckBox.setDeck(player.getHandDeck());
            topCardBox.setCard(player.getTopCard());

            ui.setPage(0);
            // player.receiveServerCommand();
            this_thread::sleep_for(chrono::milliseconds(200));
            // player.printHandDeck();
        }
    } catch(exception &e) {
        cerr<<"Server error: "<<e.what()<<endl;
    }
    
   
    return 0;
}