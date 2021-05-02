/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#pragma once
#ifndef _TIME24_H
#define _TIME24_H

#include <string>
#include <optional>
#include <iostream>

class Time24 {
    int hour;
    int minute;

    public:
    Time24(int, int);
    ~Time24() = default;

    bool setHour(int);
    bool setMinute(int);
    int getHour() const;
    int getMinute() const;

    std::string toString() const;
    friend std::ostream& operator<< (std::ostream&, const Time24&);

    void serialize(std::ostream&) const;
    static std::optional<Time24> deserialize(std::istream&);
};

#endif