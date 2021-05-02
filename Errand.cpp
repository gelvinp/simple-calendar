/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#include <string>
#include <iostream>
#include <optional>

#include "Time24.h"
#include "Errand.h"
#include "StringUtils.h"

// Reminder definitions
// Holds text, nothing else really
Reminder::Reminder(const std::string& text) : text(text) {}

void Reminder::setText(const std::string& text) {
    this->text = text;
}

std::string Reminder::toString() const { return this-> text; }

std::ostream& operator<< (std::ostream& out, const Reminder& reminder) {
    out << reminder.toString();
    return out;
}

// Serialize format:
// text

void Reminder::serialize(std::ostream& out) const {
    out << serialEncode(this->toString());
}

std::optional<Reminder*> Reminder::deserialize(std::istream& in) {
    try {
        std::string text;
        std::getline(in, text, ':');
        text = serialDecode(text);

        // If here, text read successfully
        return new Reminder { text };
    } catch (...) {
        // Deserialize failed, return empty optional
        return std::nullopt;
    }
}

// Appointment definitions
// Holds a start and end time, in addition to text
Appointment::Appointment(const std::string& text, const Time24& start, const Time24& end)
    : Reminder(text), startTime(start), endTime(end) {}

void Appointment::setStartTime(const Time24& time) {
    startTime = time; // Shallow copy is fine here
}

void Appointment::setEndTime(const Time24& time) {
    endTime = time;
}

const Time24& Appointment::getStartTime() const { return startTime; }
const Time24& Appointment::getEndTime() const { return endTime; }

std::string Appointment::toString() const {
    return { "From " + startTime.toString() + " to " + endTime.toString() + ": " + Reminder::toString() };
}

std::ostream& operator<< (std::ostream& out, const Appointment& appointment) {
    out << appointment.toString();
    return out;
}

// Serialize format:
// text:startTime:endTime

void Appointment::serialize(std::ostream& out) const {
    std::string text { serialEncode(Reminder::toString()) };
    out << text << ":";
    startTime.serialize(out);
    out << ":";
    endTime.serialize(out);
}

std::optional<Reminder*> Appointment::deserialize(std::istream& in) {
    try {
        std::string text;
        std::getline(in, text, ':');
        text = serialDecode(text);

        auto start = Time24::deserialize(in);
        auto end = Time24::deserialize(in);

        // Check Time24 Optionals
        if (!start.has_value() || !end.has_value()) {
            throw;
        }

        // If here, all inputs read successfully
        return new Appointment { text, start.value(), end.value() };
    } catch (...) {
        // Deserialize failed, return empty optional
        return std::nullopt;
    }
}