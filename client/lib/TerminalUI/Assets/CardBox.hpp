#ifndef CARD_BOX_HPP
#define CARD_BOX_HPP

#include "../Box/Box.hpp"
#include <Card/Card.hpp>

#define CARD_BOX_WIDTH 10
#define CARD_BOX_HEIGHT 7

namespace UI {

    class CardBox : public Box {
    private:
        std::string sign = "";
        Card card;

    public:
        CardBox(int x, int y) : Box(x, y, CARD_BOX_WIDTH, CARD_BOX_HEIGHT, 45, true) {}
        
        void onSelect() override {  }

        void onClick() override {  }

        void render() {
            clearSpace();
            drawBorder();
            moveCursor(getX()+getWidth()-sign.length()-1, getY()+1);
            addText(sign);
            moveCursor(getX()+1, getY()+getHeight()-2);
            addText(sign);
        }

        void resetCard() {
            setBackgroundColor(45);
            sign = "";
        }

        Card getCard() const {
            return card;
        }

        void setCard(const Card &card) {
            this->card = card;
            switch (card.getColor()) {
            case Color::RED:
                setBackgroundColor(41);
                break;
            
            case Color::BLUE:
                setBackgroundColor(44);
                break;

            case Color::GREEN:
                setBackgroundColor(42);
                break;

            case Color::YELLOW:
                setBackgroundColor(43);
                break;
            
            default:
                setBackgroundColor(40);
                break;
            }

            switch (card.getType()) {
            case Type::NUMBER:
                sign = std::to_string(card.getNumber());
                break;

            case Type::DRAW_TWO:
                sign = "+2";
                break;

            case Type::WILD_DRAW_FOUR:
                sign = "+4";
                break;

            case Type::SKIP:
                sign = "skip";
                break;

            case Type::REVERSE:
                sign = "reverse";
                break;

            case Type::WILD:
                sign = "wild";
                break;
            
            default:
                break;
            }
        }
    };
}

#endif
