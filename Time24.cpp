/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#include <string>
#include <iostream>
#include <optional>

#include "Time24.h"
#include "StringUtils.h"

// Time24 Definitions
// Class holds information on a time in 24 hour format with sanity checks
Time24::Time24(int hour, int minute) {
    if (hour < 0 || hour > 24) {
        hour = 0;
    }

    if (minute < 0 || minute > 59) {
        minute = 0;
    }

    this->hour = hour;
    this->minute = minute;
}

bool Time24::setHour(int hour) {
    if (hour < 0 || hour > 24) {
        return false;
    } else {
        this->hour = hour;
        return true;
    }
}

bool Time24::setMinute(int minute) {
    if (minute < 0 || minute > 59) {
        return false;
    } else {
        this->minute = minute;
        return true;
    }
}

int Time24::getHour() const { return this->hour; }
int Time24::getMinute() const { return this-> minute; }

std::string Time24::toString() const {
    return stringFormat("%.2i:%.2i", this->hour, this->minute);
}

std::ostream& operator<< (std::ostream& out, const Time24& time) {
    out << time.toString();
    return out;
}

// Serialize format
// hour:minute

void Time24::serialize(std::ostream& out) const {
    std::string data = stringFormat("%i:%i", this->hour, this->minute);
    out << data;
}

std::optional<Time24> Time24::deserialize(std::istream& in) {
    try {
        std::string data_s;
        std::getline(in, data_s, ':');
        int hour = std::stoi(data_s);
        std::getline(in, data_s, ':');
        int minute = std::stoi(data_s);

        // If here, both ints read successfully. Construct object
        return Time24 { hour, minute };
    } catch (...) {
        // Deserialize failed, return empty optional
        return std::nullopt;
    }
}