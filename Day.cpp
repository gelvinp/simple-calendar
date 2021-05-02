/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#include "Errand.h"
#include "Day.h"
#include "StringUtils.h"

Days nextDay(Days currentDay) {
    int currentIndex = static_cast<int>(currentDay);
    int tomorrowIndex = (currentIndex + 1) % 7;
    return static_cast<Days>(tomorrowIndex);
}

Day::Day(int num, Days day) : num(num), day(day), errands({}) {}

Day::~Day() {
    for (Reminder* i : errands) {
        delete i;
    }
    errands.clear();
}

void Day::addErrand(Reminder* const errand) {
    errands.push_back(errand);
}

bool Day::removeErrand(int index) {
    if (index < 0 || static_cast<size_t>(index) >= errands.size()) {
        return false;
    }

    delete errands.at(index);

    errands.erase(errands.begin() + index);
    return true;
}

std::string Day::toString() const {
    std::string out = stringFormat("Day %i (%s)\nYou have %i errands:\n", num, s_Days[static_cast<int>(day)].c_str(), errands.size());

    for (size_t i = 0; i < errands.size(); ++i) {
        out += stringFormat("\t[%i] %s\n", i + 1, errands.at(i)->toString().c_str());
    }

    return out;
}

std::string Day::abbrev() const {
    int rem = 0;
    int app = 0;

    for (auto i : errands) {
        if (i->type() == 'r') {
            ++rem;
        } else if (i->type() == 'a') {
            ++app;
        }
    }

    std::string dayOfWeek = s_Days[static_cast<int>(day)];

    return stringFormat("%s: %i reminders and %i appointments", dayOfWeek.c_str(), rem, app);
}

std::ostream& operator<< (std::ostream& out, const Day& day) {
    out << day.toString();
    return out;
}

// Serialize format:
// num:day:errandSize:errands
//
// Errand format:
// type:errandData

void Day::serialize(std::ostream& out) const {
    out << num <<  ":" << static_cast<int>(day) << ":" << errands.size();

    for (auto i : errands) {
        if (i->type() == 'r') {
            out << ":r:";
            i->serialize(out);
        } else if (i->type() == 'a') {
            out << ":a:";
            static_cast<Appointment*>(i)->serialize(out);
        }
    }
}

std::optional<Day*> Day::deserialize(std::istream& in) {
    Day* newDay = nullptr;
    try {
        std::string data_s;

        std::getline(in, data_s, ':');
        int num = std::stoi(data_s);

        std::getline(in, data_s, ':');
        int day_i = std::stoi(data_s);
        Days day = static_cast<Days>(day_i);

        newDay = new Day { num, day };

        std::getline(in, data_s, ':');
        int errandSize = std::stoi(data_s);

        newDay->errands.reserve(errandSize); // No need to have multiple resizes

        for (int i = 0; i < errandSize; ++i) {
            // Initialize each errand
            std::getline(in, data_s, ':');
            std::optional<Reminder*> errand;

            switch(data_s.at(0)) {
                case 'r':
                    errand = Reminder::deserialize(in);
                    break;
                case 'a':
                    errand = Appointment::deserialize(in);
                    break;
                default:
                    throw;
            }

            // Ensure deserialized correctly
            if (!errand.has_value()) {
                throw;
            }

            newDay->errands.push_back(errand.value());
        }

        // If here, all errands initalized correctly
        return newDay;
    } catch (...) {
        // Deserialize failed, return empty optional
        if (newDay != nullptr) {
            delete newDay;
        }

        return std::nullopt;
    }
}
