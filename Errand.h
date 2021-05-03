/*
Author: Patrick Gelvin
Date: April 10th, 2021
*/

#pragma once
#ifndef _APPOINTMENT_H
#define _APPOINTMENT_H

#include <string>
#include <optional>
#include <iostream>

#include "Time24.h"

class Reminder {
    std::string text;

    public:
    Reminder(const std::string&);
    virtual ~Reminder() = default;
    virtual char type() { return 'r'; } // I *know* typeof() is a thing but I could not get it to work and this was easier

    void setText(const std::string&);
    // toString is the getter for text

    virtual std::string toString() const;
    friend std::ostream& operator<< (std::ostream&, const Reminder&);

    virtual void serialize(std::ostream&) const;
    static std::optional<Reminder*> deserialize(std::istream&);
};

class Appointment: public Reminder {
    Time24 startTime, endTime;

    public:
    Appointment(const std::string&, const Time24&, const Time24&);
    ~Appointment() = default;
    char type() override { return 'a'; }

    void setStartTime(const Time24&);
    void setEndTime(const Time24&);
    const Time24& getStartTime() const;
    const Time24& getEndTime() const;

    std::string toString() const override;
    friend std::ostream& operator<< (std::ostream&, const Appointment&);

    void serialize(std::ostream&) const override;
    static std::optional<Reminder*> deserialize(std::istream&);
};

#endif
