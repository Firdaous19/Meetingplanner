//
// Created by firdi on 27/04/2026.
//
#include "Campus.h"
#include "DesignByContract.h"

Campus::Campus(const std::string& name,
               const std::string& identifier)
        : name(name), identifier(identifier) {
    REQUIRE(!name.empty(), "Campus name mag niet leeg zijn");
    REQUIRE(!identifier.empty(), "Campus identifier mag niet leeg zijn");

    ENSURE(this->name == name, "Campus name correct opgeslagen");
    ENSURE(this->identifier == identifier, "Campus identifier correct opgeslagen");
}