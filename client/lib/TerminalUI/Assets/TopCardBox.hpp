#ifndef TOP_CARD_BOX_HPP
#define TOP_CARD_BOX_HPP

#include "../Box/Box.hpp"
#include "CardBox.hpp"
#include <Card/Card.hpp>

#define TOP_CARD_BOX_WIDTH (CARD_BOX_WIDTH + 10)
#define TOP_CARD_BOX_HEIGHT (CARD_BOX_HEIGHT + 2)

namespace UI {
    class TopCardBox : public Box {
    private:
        CardBox topCard;

    public:
        TopCardBox(int x, int y) : Box(x, y, TOP_CARD_BOX_WIDTH, TOP_CARD_BOX_HEIGHT, 45, true), topCard(x+5,y+1) {
            registerBox(&topCard);
        }

        void render() {
            clearSpace();
            drawBorder();
        }

        void setCard(const Card &card) {
            topCard.setCard(card);
        }
    };
}

#endif
