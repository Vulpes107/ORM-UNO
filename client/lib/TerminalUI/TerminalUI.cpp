#include "TerminalUI.hpp"

namespace UI {
    // Setup the terminal by modifying terminal settings
    void TerminalUI::setupTerminal() {
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
            throw std::runtime_error("Can't get window size");
        }

        height = ws.ws_row;
        width = ws.ws_col;

        struct termios t;
        if (tcgetattr(STDIN_FILENO, &originalTermios) == -1) {
            throw std::runtime_error("Failed to get terminal attributes");
        }

        t = originalTermios;
        t.c_lflag &= ~(ICANON | ECHO); // Disable line buffering and echo
        if (tcsetattr(STDIN_FILENO, TCSANOW, &t) == -1) {
            throw std::runtime_error("Failed to set terminal attributes");
        }

        std::cout << "\033[?25l"; // Hide cursor
        std::cout << "\033[?7l";  // Disable line wrap
    }

    // Restore the terminal to its original state
    void TerminalUI::restoreTerminal() {
        std::cout << "\033[?7h";  // Enable line wrap
        std::cout << "\033[?25h"; // Show cursor
        if (tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios) == -1) {
            throw std::runtime_error("Failed to restore terminal attributes");
        }
    }

    // Clear the screen and reset the cursor to the home position
    void TerminalUI::clearScreen() {
        std::cout << "\033[2J\033[H"; // Clear screen and move cursor to home
    }

    void TerminalUI::keyHandlerWorker() {
        while(running) {
            Key key = keyboard.getKey();

            if (key == Key::QUIT) {
                running = false;
                break;
            }

            if(currentPage == nullptr) {
                continue;
            }

            currentPage->invokeKeyCallback(key);
        }
    }

    // Constructor: Set up terminal and clear the screen
    TerminalUI::TerminalUI() : keyboard(), running(true), currentPage(nullptr), keyWorker(&UI::TerminalUI::keyHandlerWorker, this)/**/ {
        setupTerminal();
        clearScreen();
    }

    // Destructor: Restore the terminal settings
    TerminalUI::~TerminalUI() {
        keyWorker.join();
        restoreTerminal();
    }

    bool TerminalUI::isRunning() {
        return running;
    }

    void TerminalUI::registerPage(Page *page) {
        if(page != nullptr) {
            pages.push_back(page);
        }
    }

    unsigned short TerminalUI::getWidth() const {
        return width;
    }

    unsigned short TerminalUI::getHeight() const {
        return height;
    }

    void TerminalUI::setPage(int pageNum) {
        currentPage = pages.at(pageNum);
        currentPage->render();
    }

}