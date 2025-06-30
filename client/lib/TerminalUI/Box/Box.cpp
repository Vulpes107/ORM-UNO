#include "Box.hpp"
#include <stdexcept>

namespace UI {

    std::mutex Box::coutMutex; // Static mutex for std::cout

    // Constructor
    Box::Box(int x, int y, int width, int height, int backgroundColor, bool selectable)
        : x(x), y(y), width(width), height(height), selectable(selectable), selected(false), backgroundColor(backgroundColor) {}

    std::string Box::getBackgroundANSI() const {
        // std::lock_guard<std::mutex> lock(boxMutex); // Protect access
        return "\033[" + std::to_string(backgroundColor + ((selected) ? 60 : 0)) + "m";
    }

    // Add text with background color
    void Box::addText(std::string str) {
        std::lock_guard<std::mutex> lock(boxMutex); // Protect parameters
        std::lock_guard<std::mutex> lockCout(coutMutex); // Protect cout
        std::cout << getBackgroundANSI(); // ANSI background color
        std::cout << str;
    }

    // Update the space for the box with the background color
    void Box::clearSpace() {
        std::lock_guard<std::mutex> lock(boxMutex); // Protect parameters
        std::lock_guard<std::mutex> lockCout(coutMutex); // Protect cout
        if (backgroundColor >= 0) {
            std::cout << getBackgroundANSI(); // ANSI background color
        }
        for (int row = 0; row < height; ++row) {
            // moveCursor(x, y + row);
            std::cout << "\033[" << y+row << ";" << x << "H";
            std::cout << std::string(width, ' ');
        }
        std::cout << "\033[0m"; // Restore color
    }

    void Box::drawBorder() {
        std::lock_guard<std::mutex> lock(boxMutex); // Protect parameters
        std::lock_guard<std::mutex> lockCout(coutMutex); // Protect cout
        // Draw top edge
        std::cout << "\033[" << y << ";" << x << "H"; // Move to top-left corner
        std::cout << getBackgroundANSI() << "┌"; // Top-left corner
        for (int i = 0; i < width - 2; ++i) {
            std::cout << "─"; // Horizontal line
        }
        std::cout << "┐"; // Top-right corner

        // Draw sides
        for (int i = 1; i < height - 1; ++i) {
            std::cout << "\033[" << (y + i) << ";" << x << "H"; // Move to left side
            std::cout << "│"; // Left vertical line
            std::cout << "\033[" << (y + i) << ";" << (x + width - 1) << "H"; // Move to right side
            std::cout << "│"; // Right vertical line
        }

        // Draw bottom edge
        std::cout << "\033[" << (y + height - 1) << ";" << x << "H"; // Move to bottom-left corner
        std::cout << "└"; // Bottom-left corner
        for (int i = 0; i < width - 2; ++i) {
            std::cout << "─"; // Horizontal line
        }
        std::cout << "┘"; // Bottom-right corner

        // Reset color
        std::cout << "\033[0m";
    }

    // Move the cursor to a specific position
    void Box::moveCursor(int col, int row) {
        std::lock_guard<std::mutex> lockCout(coutMutex); // Protect cout
        std::cout << "\033[" << row << ";" << col << "H";
    }

    // Setters
    void Box::setX(int x) { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        this->x = x; 
    }
    void Box::setY(int y) { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        this->y = y; 
    }
    void Box::setWidth(int width) { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        this->width = width; 
    }
    void Box::setHeight(int height) { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        this->height = height; 
    }
    void Box::setSelectable(bool selectable) { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        this->selectable = selectable; 
    }
    void Box::setBackgroundColor(int color) { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        backgroundColor = color; 
    }
    void Box::select() { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        if (selectable) selected = true; 
    }
    void Box::unselect() { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        selected = false; 
    }

    // Getters
    bool Box::isSelected() { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        return selected; 
    }
    int Box::getX() const { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        return x; 
    }
    int Box::getY() const { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        return y; 
    }
    int Box::getWidth() const { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        return width; 
    }
    int Box::getHeight() const { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        return height; 
    }
    bool Box::isSelectable() const { 
        std::lock_guard<std::mutex> lock(boxMutex); 
        return selectable; 
    }

    void Box::addKeyCallback(Key key, const std::function<void()>& callback) {
        if (key == Key::QUIT || key == Key::UNKNOWN) {
            throw std::invalid_argument("Cannot add callbacks for QUIT or UNKNOWN keys.");
        }
        std::lock_guard<std::mutex> lock(boxMutex);
        keyCallbacks[key] = callback;
    }

    void Box::removeKeyCallback(Key key) {
        std::lock_guard<std::mutex> lock(boxMutex);
        keyCallbacks.erase(key);
    }

    void Box::invokeKeyCallback(Key key) {
        std::lock_guard<std::mutex> lock(boxMutex);
        if (keyCallbacks.count(key)) {
            keyCallbacks[key](); // Invoke the callback
        }
    }

    void Box::registerBox(Box* box) {
        std::lock_guard<std::mutex> lock(boxMutex);
        boxes.push_back(box);
    }

    std::unordered_map<Key, std::function<void()>> Box::getKeyCallbacks() const {
        std::lock_guard<std::mutex> lock(boxMutex);
        return keyCallbacks;
    }

    std::vector<Box*> Box::getBoxes() const {
        std::lock_guard<std::mutex> lock(boxMutex);
        return boxes;
    }
}
