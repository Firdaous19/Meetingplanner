#include "Room.h"

Room::Room(const std::string& name, int capacity) : name(name), capacity(capacity) {
    REQUIRE(!name.empty(), "Kamer moet een naam hebben");
    REQUIRE(capacity > 0, "Capaciteit moet groter zijn dan 0");
}
