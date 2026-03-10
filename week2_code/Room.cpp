#include "Room.h"
#include <stdexcept>

Room::Room(const std::string& name, int capacity) : name(name), capacity(capacity) {
    if (capacity <= 0) {
        throw std::runtime_error("Capaciteit moet groter zijn dan 0");
    }
}

void Room::addPerson(std::string name) {
    if (persons.size() >= (size_t)capacity) {
        throw std::runtime_error("Kamer is al vol!");
    }

    persons.push_back(name);
}