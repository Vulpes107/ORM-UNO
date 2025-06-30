#ifndef BOX_HPP
#define BOX_HPP

#include <iostream>
#include <string>
#include <functional>
#include <mutex>
#include "../KeyHandler/KeyHandler.hpp"

namespace UI {
    class Box {
    private:
        unsigned short x, y;
        unsigned short width, height;
        bool selectable;
        bool selected;
        int backgroundColor;
        std::unordered_map<Key, std::function<void()>> keyCallbacks;
        std::vector<Box*> boxes;
        mutable std::mutex boxMutex;
        static std::mutex coutMutex;
        std::string getBackgroundANSI() const;

    protected:
        // bool stringFitsBox(const std::string& input);
        void addText(std::string str);

        void addKeyCallback(Key key, const std::function<void()>& callback);
        void removeKeyCallback(Key key);
        void registerBox(Box* box);

    public:
        Box(int x, int y, int width, int height, int backgroundColor = 0, bool selectable = false);
        virtual ~Box() = default;

        void invokeKeyCallback(Key key);
        virtual void onSelect() {};
        virtual void onClick() {};
        virtual void render() = 0;

        void clearSpace();
        void drawBorder();
        void moveCursor(int col, int row);

        void setX(int x);
        void setY(int y);
        void setWidth(int width);
        void setHeight(int height);
        void setBackgroundColor(int color);
        void setSelectable(bool selectable);
        void select();
        void unselect();

        bool isSelected();
        int getX() const;
        int getY() const;
        int getWidth() const;
        int getHeight() const;
        bool isSelectable() const;

        std::unordered_map<Key, std::function<void()>> getKeyCallbacks() const;
        std::vector<Box*> getBoxes() const;
    };
}

#endif // BOX_HPP
