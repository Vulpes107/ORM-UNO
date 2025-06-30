import os
import time

def clear_screen():
    """Clear the terminal screen."""
    # os.system('cls' if os.name == 'nt' else 'clear')
    print(f"\033[2J\033[H")


def print_at(x, y, message):
    """
    Prints a message at the specified (x, y) coordinates.
    :param x: Horizontal position (column) starting from 1.
    :param y: Vertical position (row) starting from 1.
    :param message: The message to print.
    """
    # Move cursor to (x, y) and print the message
    print(f"\033[{y};{x}H{message}")

def main():
    """Main function to take input and print the message."""
    clear_screen()
    print("Enter the message you'd like to display:")
    message = input("> ")
    print("\nEnter the coordinates where you'd like the message to appear.")
    x = int(input("Column (x): "))
    y = int(input("Row (y): "))
    
    # Clear screen and print the message at (x, y)
    for i in range(4):
        clear_screen()
        print_at(x+i, y+i, message)
        time.sleep(1)

if __name__ == "__main__":
    main()
