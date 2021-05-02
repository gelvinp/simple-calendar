/*
Author: Patrick Gelvin
Date: April 10th, 2021
Description: Assignment 5 -Implement a calendar in c++
*/

#pragma once
#ifndef _MENU_H
#define _MENU_H

/*
Okay this is probably gonna be a hot mess
but it is 3:48 am and i'm losing it
so yeah
~bon apetit~

I want to do my menu in a sorta half baked FSM
I *will* regret this
Things that are quick (asking 0 or 1 questions) can stay in the same state
Any more and I'll transition

States I want include:
Unloaded (either start a new cal or open an existing one)
Create (Create a new calendar)
TopMenu (Calendar is open)
DayMenu (Modify/View a specific day)
AddErrand (Add an errand to the calendar)
RemoveErrand (Remove [and verify] an errand from the calendar)
*/

class Machine;
class Calendar;
class Day;

class State {
    public:
    ~State() = default;
    virtual bool run(Machine* const) = 0;
};

class stateUnloaded: public State {
    bool run(Machine* const);
    void loadCalendar(Machine* const);
};

class stateCreate: public State {
    bool run(Machine* const);
};

class stateTopMenu: public State {
    bool run(Machine* const);
    void selectDay(Machine* const);
    void saveCalendar(Machine* const);
    void print(Machine* const);
    bool verifyUnload();
};

class stateDayMenu: public State {
    bool run(Machine* const);
};

class stateAddErrand: public State {
    bool run(Machine* const);
};

class stateRemoveErrand: public State {
    bool run(Machine* const);
};


class Machine {
    friend stateUnloaded;
    friend stateCreate;
    friend stateTopMenu;
    friend stateDayMenu;
    friend stateAddErrand;
    friend stateRemoveErrand;

    stateUnloaded unloaded;
    stateCreate create;
    stateTopMenu topMenu;
    stateDayMenu dayMenu;
    stateAddErrand addErrand;
    stateRemoveErrand removeErrand;

    State* activeState;
    Calendar* activeCal;
    int activeDay;
    bool unsaved;

    Machine();
    ~Machine();

    static int getInt(const char* const);
    static int getInt(const char* const, int, int);

    bool loadCalendar(std::ifstream&);

    public:
    static void run();
    static void run(const char* const);
};

#endif