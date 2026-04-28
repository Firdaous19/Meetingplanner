#include "Renovation.h"
#include "DesignByContract.h"

Renovation::Renovation(const std::string& roomIdentifier,
                       const std::string& startDate,
                       const std::string& endDate)
        : roomIdentifier(roomIdentifier),
          startDate(startDate),
          endDate(endDate) {

    REQUIRE(!roomIdentifier.empty(), "Renovation room identifier mag niet leeg zijn");
    REQUIRE(!startDate.empty(), "Renovation start date mag niet leeg zijn");
    REQUIRE(!endDate.empty(), "Renovation end date mag niet leeg zijn");

    ENSURE(this->roomIdentifier == roomIdentifier, "Room identifier correct opgeslagen");
    ENSURE(this->startDate == startDate, "Start date correct opgeslagen");
    ENSURE(this->endDate == endDate, "End date correct opgeslagen");
}
