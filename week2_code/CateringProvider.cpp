//
// Created by firdi on 28/04/2026.
//
#include "CateringProvider.h"
#include "DesignByContract.h"

CateringProvider::CateringProvider(const std::string& campusIdentifier,
                                   int co2)
        : campusIdentifier(campusIdentifier),
          co2(co2) {

    REQUIRE(!campusIdentifier.empty(), "Campus identifier mag niet leeg zijn");
    REQUIRE(co2 >= 0, "CO2 mag niet negatief zijn");

    ENSURE(this->campusIdentifier == campusIdentifier,
           "Campus identifier correct opgeslagen");
    ENSURE(this->co2 == co2,
           "CO2 correct opgeslagen");
}