/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#include <string>
#include <regex>
#include <iostream>
#include <limits>

#include "StringUtils.h"

// Since in my serialization I'm using : as a delimiter,
// I gotta encode my user entered text to make sure that :
// doesn't appear in the output
std::string serialEncode(const std::string& in) {
    return std::regex_replace(in, std::regex(":"), R"(\[col])");
}

std::string serialDecode(const std::string& in) {
    return std::regex_replace(in, std::regex(R"(\\\[col\])"), ":");
}

char getChar(const char* const prompt) {
    char ch;
    while(true) {
        std::cout << prompt;
        std::cin >> ch;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        } else break;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return ch;
}