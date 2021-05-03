/*
Author: Patrick Gelvin
Date: April 10th, 2021
*/

#pragma once
#ifndef _DAY_H
#define _DAY_H

#include <vector>
#include <string>
#include <iostream>
#include <optional>

class Reminder;

enum Days {
    Monday = 0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

// Indices correspond to enum
static const std::string s_Days[7] { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

Days nextDay(Days);

class Day {
    int num;
    Days day;
    std::vector<Reminder*> errands;

    public:
    Day(int, Days);
    ~Day();

    void addErrand(Reminder* const);
    bool removeErrand(int);

    int getNum() const { return num; };
    int getErrandCount() const { return errands.size(); };

    std::string toString() const;
    std::string abbrev() const; // Outputs abbreviated day (number of reminders and number of appointments only)
    friend std::ostream& operator<< (std::ostream&, const Day&);

    void serialize(std::ostream&) const;
    static std::optional<Day*> deserialize(std::istream&);
};

#endif
