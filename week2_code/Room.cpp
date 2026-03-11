#include "Room.h"
#include <stdexcept>
#include "DesignByContract.h"

Room::Room(const std::string& name, int capacity) : name(name), capacity(capacity) {
    REQUIRE(capacity > 0, "Capaciteit moet groter zijn dan 0");
}

void Room::addPerson(std::string name) {
    REQUIRE(persons.size() < (size_t)capacity, "Kamer is al vol!");
    persons.push_back(name);
}


