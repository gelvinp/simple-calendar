/*
Author: Patrick Gelvin
Date: April 10th, 2021
*/

#pragma once
#ifndef _CALENDAR_H
#define _CALENDAR_H

#include <string>
#include <optional>
#include <iostream>

#include "Day.h"

class Calendar {
    std::string title;
    int numDays;
    Day** days;
    Calendar() = default; // Private constructor for deserialization

    public:
    Calendar(const std::string&, int, Days);
    ~Calendar();

    std::string dayAgenda(int) const;
    std::string toString() const;

    std::string getTitle() const { return title; }
    int getNumDays() const { return numDays; }

    void print(std::ostream&) const; // Prints detailed calendar to stream
    Day* fetchDay(int);

    friend std::ostream& operator<< (std::ostream&, const Calendar&);

    void serialize(std::ostream&) const;
    static std::optional<Calendar*> deserialize(std::istream&);
};

#endif
