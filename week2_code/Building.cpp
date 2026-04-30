
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