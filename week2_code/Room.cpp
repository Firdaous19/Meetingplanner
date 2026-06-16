#include "Room.h"
#include "DesignByContract.h"

Room::Room(const std::string& name,
           const std::string& identifier,
           int capacity,
           const std::string& campusIdentifier,
           const std::string& buildingIdentifier)
        : name(name),
          identifier(identifier),
          capacity(capacity),
          campusIdentifier(campusIdentifier),
          buildingIdentifier(buildingIdentifier) {
    REQUIRE(!name.empty(), "Room name mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Room identifier mag niet leeg zijn");
    REQUIRE(capacity > 0, "Capaciteit moet groter zijn dan 0");
    REQUIRE(!campusIdentifier.empty(), "Room campus identifier mag niet leeg zijn");
    REQUIRE(!buildingIdentifier.empty(), "Room building identifier mag niet leeg zijn");

    ENSURE(this->name == name, "Room name correct opgeslagen");
    ENSURE(this->identifier == identifier, "Room identifier correct opgeslagen");
    ENSURE(this->capacity == capacity, "Room capacity correct opgeslagen");
    ENSURE(this->campusIdentifier == campusIdentifier,
           "Room campus identifier correct opgeslagen");
    ENSURE(this->buildingIdentifier == buildingIdentifier,
           "Room building identifier correct opgeslagen");
    ENSURE(!occupied, "Nieuwe room mag niet bezet zijn");
}

void Room::addPerson(const std::string& personName) {
    REQUIRE(!personName.empty(), "Persoonsnaam mag niet leeg zijn");
    REQUIRE(persons.size() < static_cast<size_t>(capacity), "Kamer is al vol!");

    size_t oldSize = persons.size();
    persons.push_back(personName);

    ENSURE(persons.size() == oldSize + 1, "Persoon moet toegevoegd zijn");
}

void Room::occupy() {
    REQUIRE(!occupied, "Room mag niet al bezet zijn");

    occupied = true;

    ENSURE(occupied, "Room moet bezet zijn na occupy()");
}
void Room::addRenovation(const Renovation& renovation) {
    REQUIRE(renovation.getRoomIdentifier() == identifier,
            "Renovation moet bij deze room horen");

    size_t oldSize = renovations.size();
    renovations.push_back(renovation);

    ENSURE(renovations.size() == oldSize + 1,
           "Renovation moet toegevoegd zijn aan room");
}

bool Room::isBeingRenovated(const std::string& date) const {
    REQUIRE(!date.empty(), "Datum mag niet leeg zijn");

    for (const Renovation& renovation : renovations) {
        if (renovation.isActiveOnDate(date)) {
            return true;
        }
    }

    return false;
}