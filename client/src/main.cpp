#include <iostream>
#include <string.h>
#include <Client.hpp>
#include <Player.hpp>
#include <unordered_map>
#include <csignal>
//#include <TerminalUI/TerminalUI.hpp>
#include <thread>


using namespace std;
//using namespace UI;


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

    try {
        Client client;
        client.connectToServer();

        Player player(username, client);

        player.setHandDeck();
        player.printHandDeck();
        
        while(!player.getEndGame()) {
            player.receiveServerCommand();
        }
    } catch(exception &e) {
        cerr<<"Server error: "<<e.what()<<endl;
    }
    
   
    return 0;
}