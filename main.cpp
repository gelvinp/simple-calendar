/*
Author: Patrick Gelvin
Date: April 10th, 2021
Usage:
    ./calendar
        Opens the program normally

    /calendar [filename]
        Opens the program and tries to read the calendar stored in filename
*/

#include <iostream>

#include "Menu.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        // Open program normally
        Machine::run();
    } else if (argc == 2) {
        Machine::run(argv[1]);
    } else {
        std::cout << "Usage: ./calendar [filename]\n"
                    "filename - optional parameter, specifies name of calendar file to load\n";
    }

    return 0;
}
