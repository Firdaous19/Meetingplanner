//
// Created by firdi on 5/03/2026.
//
#include "Room.h"
#include "DesignByContract.h"

Room::Room(const std::string& name, int capacity) : name(name), capacity(capacity) {
    REQUIRE(!name.empty(), "Kamer moet een naam hebben");
    REQUIRE(capacity > 0, "Capaciteit moet groter zijn dan 0");
}
