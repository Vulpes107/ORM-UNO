#ifndef PLAYER_BOX_HPP
#define PLAYER_BOX_HPP

#include "../Box/Box.hpp"

#define PLAYER_BOX_WIDTH 20
#define PLAYER_BOX_HEIGHT 4

namespace UI {
    class PlayerBox : public Box {
    public:
        PlayerBox(int x, int y) : Box(x, y, PLAYER_BOX_WIDTH, PLAYER_BOX_HEIGHT, 46, false) {}

        void render() {
            clearSpace(); 
            drawBorder();
        }
    };
}

#endif
