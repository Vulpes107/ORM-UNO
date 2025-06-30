#ifndef DECK_BOX_HPP
#define DECK_BOX_HPP 

#include "../Box/Box.hpp"
#include "CardBox.hpp"
#include <Card/Card.hpp>
#include <memory>
#include <mutex>

#define DECK_BOX_WIDTH (CARD_BOX_WIDTH * 7)
#define DECK_BOX_HEIGHT (CARD_BOX_HEIGHT * 3)

namespace UI {
    class DeckBox : public Box {
    private:
        int selectedCardIndexRow;
        int selectedCardIndexCol;
        std::vector<std::vector<std::shared_ptr<CardBox>>> cardBoxes;
        std::mutex mx;

        static constexpr int calculateMaxCols(int containerWidth, int rectWidth) {
            return containerWidth / rectWidth;
        }

        static constexpr int calculateMaxRows(int containerHeight, int rectHeight) {
            return containerHeight / rectHeight;
        }

    public:
        DeckBox(int x, int y)
            : Box(x, y, DECK_BOX_WIDTH, DECK_BOX_HEIGHT),
              selectedCardIndexRow(0),
              selectedCardIndexCol(0),
              cardBoxes(calculateMaxRows(DECK_BOX_HEIGHT, CARD_BOX_HEIGHT),
                        std::vector<std::shared_ptr<CardBox>>(calculateMaxCols(DECK_BOX_WIDTH, CARD_BOX_WIDTH), nullptr)) {

            size_t i = 0, j = 0;
            for (auto &cardRow : cardBoxes) {
                for (auto &cardBox : cardRow) {
                    cardBox = std::make_shared<CardBox>(getX() + CARD_BOX_WIDTH * j, getY() + CARD_BOX_HEIGHT * i);
                    registerBox(cardBox.get());
                    ++j;
                }
                j = 0;
                ++i;
            }

            cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->select();

            addKeyCallback(Key::ARROW_LEFT, [this]() {
                std::lock_guard<std::mutex> lock(mx);
                if (selectedCardIndexCol > 0 && cardBoxes[selectedCardIndexRow][selectedCardIndexCol - 1]->isSelectable()) {
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->unselect();
                    --selectedCardIndexCol;
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->select();
                }
            });

            addKeyCallback(Key::ARROW_RIGHT, [this]() {
                std::lock_guard<std::mutex> lock(mx);
                if (selectedCardIndexCol < calculateMaxCols(DECK_BOX_WIDTH, CARD_BOX_WIDTH) - 1 &&
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol + 1]->isSelectable()) {
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->unselect();
                    ++selectedCardIndexCol;
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->select();
                }
            });

            addKeyCallback(Key::ARROW_DOWN, [this]() {
                std::lock_guard<std::mutex> lock(mx);
                if (selectedCardIndexRow < calculateMaxRows(DECK_BOX_HEIGHT, CARD_BOX_HEIGHT) - 1 &&
                    cardBoxes[selectedCardIndexRow + 1][selectedCardIndexCol]->isSelectable()) {
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->unselect();
                    ++selectedCardIndexRow;
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->select();
                }
            });

            addKeyCallback(Key::ARROW_UP, [this]() {
                std::lock_guard<std::mutex> lock(mx);
                if (selectedCardIndexRow > 0 && cardBoxes[selectedCardIndexRow - 1][selectedCardIndexCol]->isSelectable()) {
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->unselect();
                    --selectedCardIndexRow;
                    cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->select();
                }
            });

            addKeyCallback(Key::ENTER, [this]() {
                std::lock_guard<std::mutex> lock(mx);
                auto card = cardBoxes[selectedCardIndexRow][selectedCardIndexCol]->getCard();
                std::cout<<card.toString()<<std::endl;
            });
        }

        void render() {}

        void setDeck(std::vector<Card> handDeck) {
            std::lock_guard<std::mutex> lock(mx);
            size_t i = 0;
            for (auto &cardRow : cardBoxes) {
                for (auto &cardBox : cardRow) {
                    if (i < handDeck.size()) {
                        cardBox->setCard(handDeck[i++]);
                        cardBox->setSelectable(true);
                    } else {
                        cardBox->resetCard();
                        cardBox->setSelectable(false);
                    }
                }
            }
        }
    };
}

#endif
