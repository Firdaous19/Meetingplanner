//
// Created by firdi on 28/04/2026.
//
#include "CateringProvider.h"
#include "DesignByContract.h"

CateringProvider::CateringProvider(const std::string& campusIdentifier,
                                   float co2)
        : campusIdentifier(campusIdentifier),
          co2(co2) {

    REQUIRE(!campusIdentifier.empty(), "Campus identifier mag niet leeg zijn");
    REQUIRE(co2 > 0, "CO2 moet groter zijn dan 0");

    ENSURE(this->campusIdentifier == campusIdentifier,
           "Campus identifier correct opgeslagen");
    ENSURE(this->co2 == co2,
           "CO2 correct opgeslagen");
}
std::string CateringProvider::getCampusIdentifier() const {
    ENSURE(!campusIdentifier.empty(),
           "Campus identifier mag niet leeg zijn");
    return campusIdentifier;
}

float CateringProvider::getCO2() const {
    ENSURE(co2 > 0,
           "CO2 moet groter zijn dan 0");
    return co2;
}