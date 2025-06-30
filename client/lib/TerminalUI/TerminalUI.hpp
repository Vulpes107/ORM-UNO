#ifndef TERMINAL_UI_HPP
#define TERMINAL_UI_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "Page/Page.hpp"
#include "KeyHandler/KeyHandler.hpp"
#include <thread>

namespace UI {

    class TerminalUI {
    private:
        KeyHandler keyboard;
        struct termios originalTermios;  // Store original terminal settings
        bool running;
        Page *currentPage;
        std::vector<Page*> pages;        // List of pages to display
        unsigned short width, height;    // Terminal window dimensions
        std::thread keyWorker;

        // Private methods for terminal management
        void setupTerminal();            // Set up terminal settings
        void restoreTerminal();          // Restore original terminal settings
        void clearScreen();              // Clear the terminal screen
        void keyHandlerWorker();

    public:
        // Constructor & Destructor
        TerminalUI();
        ~TerminalUI();

        bool isRunning();

        void registerPage(Page *page);
        unsigned short getWidth() const;
        unsigned short getHeight() const;
        
        void setPage(int pageNum);
    };

}

#endif // TERMINAL_UI_HPP
