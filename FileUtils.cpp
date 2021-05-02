/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>

#include "FileUtils.h"
#include "StringUtils.h"

std::optional<std::ofstream*> openForWrite(const std::string filename) {
    try {
        std::filesystem::path path { filename };

        // Check for overwrite
        if (std::filesystem::exists(path)) {
            std::cout << filename << " exists already and will be overwritten. Enter Y to confirm.\n";
            char ch = getChar("?> ");
            if (ch != 'y' && ch != 'Y') {
                return std::nullopt;
            }
        }

        std::ofstream* out = new std::ofstream;
        out->open(path, std::ios::out | std::ios::trunc);

        return out;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<std::ifstream*> openForRead(const std::string filename) {
    try {
        std::filesystem::path path { filename };

        // Check file exists
        if (!std::filesystem::exists(path)) {
            std::cout << filename << " does not exist.\n";
            return std::nullopt;
        }

        std::ifstream* in = new std::ifstream;
        in->open(path);

        return in;
    } catch (...) {
        return std::nullopt;
    }
}