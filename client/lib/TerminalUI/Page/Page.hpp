#ifndef PAGE_HPP
#define PAGE_HPP

#include <vector>
#include <stdexcept>  // For std::runtime_error
#include <functional>
#include <map>
#include <mutex>

#include "../Box/Box.hpp"    // Include the Box class definition
#include "../KeyHandler/KeyHandler.hpp"

namespace UI {

    class Page {
    private:
        std::vector<Box*> boxes;
        Box *focusedBox = nullptr;
        // std::unordered_map<Key, std::function<void()>> keyCallbacks;
        std::mutex mx;

        // Private method to check if two boxes overlap
        static bool boxesOverlap(Box* a, Box* b);

    public:
        // Method to register a Box, ensuring no overlap with existing ones
        void registerBox(Box* box);
        void focusBox(Box* box);
        void render();
        // void addKeyCallback(Key key, const std::function<void()>& callback);
        // void removeKeyCallback(Key key);
        void invokeKeyCallback(Key key);
    };

}

#endif // PAGE_HPP
