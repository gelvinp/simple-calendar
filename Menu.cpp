/*
Author: Patrick Gelvin
Date: April 10th, 2021
*/

#include <iostream>
#include <limits>
#include <filesystem>
#include <fstream>
#include <optional>
#include <cassert>

#include "Menu.h"
#include "Calendar.h"
#include "Day.h"
#include "Errand.h"
#include "FileUtils.h"
#include "StringUtils.h"

Machine::Machine()
    : unloaded(stateUnloaded()), create(stateCreate()),topMenu(stateTopMenu()),
    dayMenu(stateDayMenu()), addErrand(stateAddErrand()),
    removeErrand(stateRemoveErrand()), activeState(&unloaded),
    activeCal(nullptr), activeDay(-1), unsaved(false) {}

Machine::~Machine() {
    if (activeCal != nullptr) {
        delete activeCal;
    }
}

void Machine::run() {
    Machine machine;
    while (machine.activeState->run(&machine)) {}
}

void Machine::run(const char* const filename) {
    Machine machine;
    auto in = openForRead(filename);

    if (in.has_value()) {
        bool loaded = machine.loadCalendar(*(in.value()));
        in.value()->close();
        delete in.value();

        if (loaded) {
            while (machine.activeState->run(&machine)) {}
        }
    }
}

int Machine::getInt(const char* const prompt) {
    int val;
    while(true) {
        std::cout << prompt;
        std::cin >> val;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // I hate how verbose this is too.
            continue;
        } else break;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Don't trust the user, ever

    return val;
}

int Machine::getInt(const char* const prompt, int min, int max) {
    int choice;
    do {
        choice = Machine::getInt(prompt);
    } while (choice < min || choice > max);

    return choice;
}

bool Machine::loadCalendar(std::ifstream& in) {
    auto cal = Calendar::deserialize(in);

    if (cal.has_value()) {
        activeCal = cal.value();
        activeState = &topMenu;
        return true;
    } else {
        std::cout << "\nThe file could not be read.\n";
        return false;
    }
}

bool stateUnloaded::run(Machine* const machine) {
    std::cout << "Hello! Please choose an option:\n\n"
                    "[1] Start a new calendar\n"
                    "[2] Open an existing calendar\n"
                    "[3] Quit\n\n";
    int choice = Machine::getInt("?> ", 1, 3);

    switch (choice) {
        case 1:
            machine->activeState = &(machine->create);
            break;
        case 2:
            loadCalendar(machine);
            break;
        case 3:
            return false;
    }
    return true;
}

void stateUnloaded::loadCalendar(Machine* const machine) {
    std::cout << "\nEnter a filename to load the calendar from\n?> ";
    std::string filename;
    std::getline(std::cin, filename);

    auto in = openForRead(filename);

    if (in.has_value()) {
        machine->loadCalendar(*(in.value()));
        in.value()->close();
        delete in.value();

        return;
    }

    machine->activeState = &(machine->unloaded);
}

bool stateCreate::run(Machine* const machine) {
    std::cout << "\nWhat would you like to call your calendar?\n?> ";
    std::string name;
    std::getline(std::cin, name);

    std::cout << "How many days should be in your calendar?\n";
    int days;
    do {
        days = Machine::getInt("?> ");
    } while (days < 1);

    std::cout << "What day of the week should your calendar start on?\n\n"
                    "[1] Monday\n"
                    "[2] Tuesday\n"
                    "[3] Wednesday\n"
                    "[4] Thursday\n"
                    "[5] Friday\n"
                    "[6] Saturday\n"
                    "[7] Sunday\n\n";

    int choice = Machine::getInt("?> ", 1, 7);

    Days startDay = static_cast<Days>(choice - 1);

    machine->activeCal = new Calendar(name, days, startDay);
    machine->activeState = &(machine->topMenu);
    machine->unsaved = true;

    return true;
}

bool stateTopMenu::run(Machine* const machine) {
    assert(machine->activeCal != nullptr);

    std::cout << "\n[" << machine->activeCal->getTitle() << "]\n"
                    "What would you like to do?\n\n"
                    "[1] Display Calendar\n"
                    "[2] Select Day\n"
                    "[3] Print Calendar (to file)\n"
                    "[4] Save Calendar\n"
                    "[5] Close Calendar\n\n";

    int choice = Machine::getInt("?> ", 1, 5);

    switch (choice) {
        case 1:
            std::cout << "\n" << *(machine->activeCal) << "\n";
            break;
        case 2:
            selectDay(machine);
            break;
        case 3:
            print(machine);
            break;
        case 4:
            saveCalendar(machine);
            break;
        case 5:
            if (machine->unsaved) {
                if (!verifyUnload()) {
                    break;
                }
            }
            delete machine->activeCal;
            machine->activeCal = nullptr;
            machine->activeState = &(machine->unloaded);
            break;
    }
    return true;
}

void stateTopMenu::selectDay(Machine* const machine) {
    assert(machine->activeCal != nullptr);

    std::cout << "\nWhich day would you like to view? (1-" << machine->activeCal->getNumDays() << ")\n";

    int choice = Machine::getInt("?> ", 1, machine->activeCal->getNumDays());

    machine->activeDay = choice - 1;
    machine->activeState = &(machine->dayMenu);
}

void stateTopMenu::saveCalendar(Machine* const machine) {
    assert(machine->activeCal != nullptr);

    std::cout << "\nEnter a filename to save the calendar to\n?> ";
    std::string filename;
    std::getline(std::cin, filename);

    auto out = openForWrite(filename);

    if (out.has_value()) {
        machine->activeCal->serialize(*(out.value()));
        out.value()->close();
        delete out.value();
        machine->unsaved = false;
    }
}

void stateTopMenu::print(Machine* const machine) {
    assert(machine->activeCal != nullptr);

    std::cout << "\nEnter a filename to print the calendar to\n?> ";
    std::string filename;
    std::getline(std::cin, filename);

    auto out = openForWrite(filename);

    if (out.has_value()) {
        machine->activeCal->print(*(out.value()));
        out.value()->close();
        delete out.value();
    }
}

bool stateTopMenu::verifyUnload() {
    std::cout << "\nAre you sure? Your unsaved changes will be lost!\n"
                    "Enter Y to confirm.\n";
    char ch = getChar("?> ");
    if (ch != 'y' && ch != 'Y') {
        return false;
    }
    return true;
}

bool stateDayMenu::run(Machine* const machine) {
    assert(machine->activeCal != nullptr);
    assert(machine->activeDay != -1);

    std::cout <<"\n" << machine->activeCal->dayAgenda(machine->activeDay) << "\nWhat would you like to do?\n\n"
                "[1] Add Errand\n"
                "[2] Remove Errand\n"
                "[3] Return\n";

    int choice = Machine::getInt("?> ", 1, 3);

    switch(choice) {
        case 1:
            machine->activeState = &(machine->addErrand);
            break;
        case 2:
            machine->activeState = &(machine->removeErrand);
            break;
        case 3:
            machine->activeDay = -1;
            machine->activeState = &(machine->topMenu);
            break;
    }

    return true;
}

bool stateAddErrand::run(Machine* const machine) {
    assert(machine->activeCal != nullptr);
    assert(machine->activeDay != -1);

    machine->activeState = &(machine->dayMenu);
    machine->unsaved = true;

    Day* activeDay = machine->activeCal->fetchDay(machine->activeDay);

    std::cout << "\nDo you want a reminder or an appointment?\n\n"
                    "[1] Reminder\n"
                    "[2] Appointment\n"
                    "[3] Cancel\n";

    int choice = Machine::getInt("?> ", 1, 3);

    if (choice == 3) {
        return true;
    }

    std::string name;
    std::cout << "Enter a name for the " << (choice == 1 ? "reminder\n" : "appointment\n?> ");
    std::getline(std::cin, name);

    if (choice == 1) {
        activeDay->addErrand(new Reminder { name });
        return true;
    }

    int startHour, startMinute, endHour, endMinute;

    std::cout << "Enter the start hour (0-23)\n";
    startHour = Machine::getInt("?> ", 0, 23);

    std::cout << "Enter the start minute (0-59)\n";
    startMinute = Machine::getInt("?> ", 0, 59);

    std::cout << "Enter the end hour (0-23)\n";
    endHour = Machine::getInt("?> ", 0, 23);

    std::cout << "Enter the end minute (0-59)\n";
    endMinute = Machine::getInt("?> ", 0, 59);

    activeDay->addErrand(new Appointment { name, { startHour, startMinute }, { endHour, endMinute } });

    return true;
}

bool stateRemoveErrand::run(Machine* const machine) {
    assert(machine->activeCal != nullptr);
    assert(machine->activeDay != -1);

    machine->activeState = &(machine->dayMenu);

    Day* activeDay = machine->activeCal->fetchDay(machine->activeDay);

    std::cout << "\nWhich errand should be removed? (1-" << activeDay->getErrandCount() << ") (0 to cancel)\n";
    int choice = Machine::getInt("?> ", 0, activeDay->getErrandCount());

    if (choice == 0) {
        return true;
    }


    machine->unsaved = true;

    activeDay->removeErrand(choice - 1);
    return true;
}
