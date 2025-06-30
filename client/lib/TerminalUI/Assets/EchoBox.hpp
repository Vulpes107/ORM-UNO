#ifndef ECHO_BOX_HPP
#define ECHO_BOX_HPP

#include "../Box/Box.hpp"
#include <string>
#include <list>

#define ECHO_BOX_WIDTH 90
#define ECHO_BOX_HEIGHT 10

namespace UI {
    class EchoBox : public Box {
    private:
        std::list<std::string> messages;

    public:
        EchoBox(int x, int y) : Box(x, y, ECHO_BOX_WIDTH, ECHO_BOX_HEIGHT) {}

        void render() {
            clearSpace();
            drawBorder();
            
            int row = 0;
            for(std::string &message : messages) {
                moveCursor(getX() + 1, getY() + 1 + row++);
                addText(message);
            }
        }

        void addMessage(const std::string &message) {
            messages.push_back(message);
            if (messages.size() > 8) {
                messages.pop_front();
            }
        }
    };
}

#endif
