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
    std::cout << "==============================" << std::endl;
    std::cout << "     UNO Client Started       " << std::endl;
    std::cout << "==============================" << std::endl;

    string username;
    while(true) {
        cout << "[LOGIN] Input username: ";
        getline(cin, username);
        if(username.empty()) {
            cout << "[ERROR] Username cannot be empty." << endl;
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
        cerr << "[CLIENT ERROR] " << e.what() << endl;
    }
    
   
    return 0;
}