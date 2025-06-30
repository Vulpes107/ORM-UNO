#ifndef PLAYERS_BOX_HPP
#define PLAYERS_BOX_HPP 

#include "../Box/Box.hpp"
#include "PlayerBox.hpp"
#include <memory>
#include <vector>

#define PLAYERS_BOX_WIDTH PLAYER_BOX_WIDTH
#define PLAYERS_BOX_HEIGHT (PLAYER_BOX_HEIGHT * 3)

namespace UI {
    class PlayersBox : public Box {
    private:
        std::vector<std::shared_ptr<PlayerBox>> playerBoxes;

    public:
        PlayersBox(int x, int y) : Box(x, y, PLAYERS_BOX_WIDTH, PLAYERS_BOX_HEIGHT, 44, false) {
            for (int i = 0; i < 3; i++) {
                playerBoxes.emplace_back(new PlayerBox(getX(), getY() + i * 4));
            }

            for(auto &playerBox : playerBoxes) {
                registerBox(playerBox.get());
            }
        }
        
        void render() {}
    };
}

#endif