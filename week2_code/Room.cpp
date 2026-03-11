#include "Room.h"
#include "DesignByContract.h"

Room::Room(const std::string& name, const std::string& identifier, int capacity)
        : name(name), identifier(identifier), capacity(capacity) {
    REQUIRE(!name.empty(), "Room name mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Room identifier mag niet leeg zijn");
    REQUIRE(capacity > 0, "Capaciteit moet groter zijn dan 0");
}

void Room::addPerson(const std::string& personName) {
    REQUIRE(!personName.empty(), "Persoonsnaam mag niet leeg zijn");
    REQUIRE(persons.size() < (size_t)capacity, "Kamer is al vol!");
    persons.push_back(personName);
}