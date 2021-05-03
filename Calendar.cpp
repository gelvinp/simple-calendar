/*
Author: Patrick Gelvin
Date: April 10th, 2021
*/

#include <string>

#include "Calendar.h"
#include "StringUtils.h"

Calendar::Calendar(const std::string& title, int numDays, Days startingDay)
    : title(title), numDays(numDays) {
        days = new Day*[numDays];

        for (int i = 0; i < numDays; ++i) {
            days[i] = new Day(i + 1, startingDay);
            startingDay = nextDay(startingDay);
        }
}

Calendar::~Calendar() {
    for (int i = 0; i < numDays; ++i) {
        delete days[i];
    }
    delete[] days;
}

std::string Calendar::dayAgenda(int index) const {
    return days[index]->toString();
}

std::string Calendar::toString() const {
    std::string out = stringFormat("%s (%i days)\n\n", title.c_str(), numDays);

    for (int i = 0; i < numDays; ++i) {
        out += stringFormat("[%i] %s\n", i + 1, days[i]->abbrev().c_str());
    }

    return out;
}

void Calendar::print(std::ostream& out) const {
    out << title << "\n" << std::string (title.length(), '-') << "\n\n";

    for (int i = 0; i < numDays; ++i) {
        out << *(days[i]) << "\n";
    }
}

Day* Calendar::fetchDay(int index) {
    return days[index];
}

std::ostream& operator<< (std::ostream& out, const Calendar& cal) {
    out << cal.toString();
    return out;
}

// Serialize format:
// title:numDays:days

void Calendar::serialize(std::ostream& out) const {
    out << title << ":" << numDays;

    for (int i = 0; i < numDays; ++i) {
        out << ":";
        days[i]->serialize(out);
    }

    out << ":\n";
}

std::optional<Calendar*> Calendar::deserialize(std::istream& in) {
    Calendar* newCal = nullptr;
    try {
        std::string title, data_s;
        
        std::getline(in, title, ':');
        std::getline(in, data_s, ':');

        int numDays = std::stoi(data_s);

        newCal = new Calendar();
        newCal->title = title;
        newCal->numDays = numDays;
        newCal->days = new Day*[numDays];

        for (int i = 0; i < numDays; ++i) {
            auto day = Day::deserialize(in);

            if (!day.has_value()) {
                throw;
            }

            newCal->days[i] = day.value();
        }

        return newCal;
    } catch (...) {
        // Deserialization failed
        if (newCal != nullptr) {
            delete newCal;
        }
        
        return std::nullopt;
    }
}
