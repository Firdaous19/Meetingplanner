#include "Room.h"
#include "DesignByContract.h"

Room::Room(const std::string& name, const std::string& identifier, int capacity)
        : name(name), identifier(identifier), capacity(capacity) {
    REQUIRE(!name.empty(), "Room name mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Room identifier mag niet leeg zijn");
    REQUIRE(capacity > 0, "Capaciteit moet groter zijn dan 0");

    ENSURE(this->name == name, "Room name correct opgeslagen");
    ENSURE(this->identifier == identifier, "Room identifier correct opgeslagen");
    ENSURE(this->capacity == capacity, "Room capacity correct opgeslagen");
    ENSURE(!occupied, "Nieuwe room mag niet bezet zijn");
}

void Room::addPerson(const std::string& personName) {
    REQUIRE(!personName.empty(), "Persoonsnaam mag niet leeg zijn");
    REQUIRE(persons.size() < (size_t)capacity, "Kamer is al vol!");

    size_t oldSize = persons.size();
    persons.push_back(personName);

    ENSURE(persons.size() == oldSize + 1, "Persoon moet toegevoegd zijn");
}

void Room::occupy() {
    REQUIRE(!occupied, "Room mag niet al bezet zijn");

    occupied = true;

    ENSURE(occupied, "Room moet bezet zijn na occupy()");
}