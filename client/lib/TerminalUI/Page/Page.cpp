#include "Page.hpp"

namespace UI {

    // Static method to check if two boxes overlap
    bool Page::boxesOverlap(Box* a, Box* b) {
        return !(a->getX() + a->getWidth() <= b->getX() ||
                 b->getX() + b->getWidth() <= a->getX() ||
                 a->getY() + a->getHeight() <= b->getY() ||
                 b->getY() + b->getHeight() <= a->getY());
    }

    // Method to register a Box to the page
    void Page::registerBox(Box* box) {
        {
            std::lock_guard<std::mutex> lock(mx);
            boxes.push_back(box);
            // for (auto callback : box->getKeyCallbacks()) {
            //     // addKeyCallback(callback.first, callback.second);
            //     keyCallbacks[callback.first] = callback.second;
            // }
        }

        for(auto childBox : box->getBoxes()) {
            registerBox(childBox);
        }
    }

    void Page::focusBox(Box* box) {
        focusedBox = box;
    }

    void Page::render() {
        std::lock_guard<std::mutex> lock(mx);
        for(Box *box : boxes) {
            // std::cout<<"Page render"<<std::endl;
            box->render();
        }
        std::cout<<std::flush;
    }

    // void Page::addKeyCallback(Key key, const std::function<void()>& callback) {
    //     if (key == Key::QUIT || key == Key::UNKNOWN) {
    //         throw std::invalid_argument("Cannot add callbacks for QUIT or UNKNOWN keys.");
    //     }

    //     std::lock_guard<std::mutex> lock(mx);
    //     keyCallbacks[key] = callback;
    // }

    // void Page::removeKeyCallback(Key key) {
    //     std::lock_guard<std::mutex> lock(mx);
    //     keyCallbacks.erase(key);
    // }

    void Page::invokeKeyCallback(Key key) {
        std::lock_guard<std::mutex> lock(mx);
        if (focusedBox) {
            focusedBox->invokeKeyCallback(key);
        }
        // if (keyCallbacks.count(key)) {
        //     keyCallbacks[key](); // Invoke the callback
        // }
    }
}
