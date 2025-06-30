#ifndef KEY_HANDLER_HPP
#define KEY_HANDLER_HPP

// #include <sys/ioctl.h>
#include <stdexcept>
#include <unistd.h>
#include <termios.h>


namespace UI {

    enum class Key {
        ARROW_UP,
        ARROW_DOWN,
        ARROW_LEFT,
        ARROW_RIGHT,
        ENTER,
        QUIT,
        DRAW,
        SKIP,
        UNKNOWN
    };

    class KeyHandler {
    private:
        struct termios originalTermios;  // Store original terminal settings
        void setupTerminal();            // Set up terminal settings
        void restoreTerminal();          // Restore original terminal settings

    public:
        KeyHandler();
        ~KeyHandler();
        Key getKey();
    };

}

#endif