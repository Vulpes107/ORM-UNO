#include "KeyHandler.hpp"

using namespace UI;

void KeyHandler::setupTerminal() {
    struct termios t;
    if (tcgetattr(STDIN_FILENO, &originalTermios) == -1) {
        throw std::runtime_error("Failed to get terminal attributes");
    }

    t = originalTermios;
    t.c_lflag &= ~(ICANON | ECHO | ISIG); // Disable canonical mode, echo, and signal handling (e.g., Ctrl+C)
    t.c_cc[VMIN] = 1; // Minimum number of bytes for non-canonical read
    t.c_cc[VTIME] = 0; // No timeout for non-canonical read

    if (tcsetattr(STDIN_FILENO, TCSANOW, &t) == -1) {
        throw std::runtime_error("Failed to set terminal attributes");
    }
}

// Restore the terminal to its original state
void KeyHandler::restoreTerminal() {
    if (tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios) == -1) {
        throw std::runtime_error("Failed to restore terminal attributes");
    }
}

KeyHandler::KeyHandler() {
    setupTerminal();
}

KeyHandler::~KeyHandler() {
    restoreTerminal();
}

Key KeyHandler::getKey() {
    while (true) {
        char c;
        if (read(STDIN_FILENO, &c, 1) != 1) {
            continue;
        }

        if (c == '\033') { // Escape sequence
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 && seq[0] == '[') {
                if (read(STDIN_FILENO, &seq[1], 1) == 1) {
                    switch (seq[1]) {
                        case 'A': return Key::ARROW_UP;
                        case 'B': return Key::ARROW_DOWN;
                        case 'C': return Key::ARROW_RIGHT;
                        case 'D': return Key::ARROW_LEFT;
                        default: return Key::UNKNOWN;
                    }
                }
            }
        } else if (c == '\n') {
            return Key::ENTER;
        } else if (c == 'q') {
            return Key::QUIT;
        } else if (c == '\x03') {
            return Key::QUIT;
        } else if (c == 'd') {
            return Key::DRAW;
        } else if (c == 's') {
            return Key::SKIP;
        }
    }
}