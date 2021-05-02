/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#pragma once
#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <string>
#include <regex>

// God I need compilers to have better support for c++20
// I just want to use std::format but NO, literally 0 compilers
// support that yet so we're left with this hairbrained mess :(

template<typename... Args>
std::string stringFormat(const char* fmtString, Args... args) {
    int len = std::snprintf(nullptr, 0, fmtString, args...) + 1; // Get buffer length plus room for \0
    char* buf = new char[len];
    std::snprintf(buf, len, fmtString, args...);
    std::string str(buf);
    delete[] buf;
    return str;
}

std::string serialEncode(const std::string& in);
std::string serialDecode(const std::string& in);

char getChar(const char* const prompt);

#endif