
#include "Building.h"
#include "DesignByContract.h"

Building::Building(const std::string& name,
                   const std::string& identifier,
                   const std::string& campusIdentifier)
        : name(name), identifier(identifier), campusIdentifier(campusIdentifier) {
    REQUIRE(!name.empty(), "Building name mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Building identifier mag niet leeg zijn");
    REQUIRE(!campusIdentifier.empty(), "Campus identifier mag niet leeg zijn");

    ENSURE(this->name == name, "Building name correct opgeslagen");
    ENSURE(this->identifier == identifier, "Building identifier correct opgeslagen");
    ENSURE(this->campusIdentifier == campusIdentifier, "Campus identifier correct opgeslagen");
}
std::string Building::getName() const {
    ENSURE(!name.empty(), "Building name mag niet leeg zijn");
    return name;
}

std::string Building::getIdentifier() const {
    ENSURE(!identifier.empty(), "Building identifier mag niet leeg zijn");
    return identifier;
}

std::string Building::getCampusIdentifier() const {
    ENSURE(!campusIdentifier.empty(), "Campus identifier mag niet leeg zijn");
    return campusIdentifier;
}
