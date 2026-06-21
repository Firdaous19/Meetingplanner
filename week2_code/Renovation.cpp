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
    REQUIRE(startDate <= endDate, "Renovation end date moet na start date of gelijk aan start date liggen");

    ENSURE(this->roomIdentifier == roomIdentifier, "Room identifier correct opgeslagen");
    ENSURE(this->startDate == startDate, "Start date correct opgeslagen");
    ENSURE(this->endDate == endDate, "End date correct opgeslagen");
    ENSURE(this->startDate <= this->endDate, "Start en end date moeten correct opgeslagen en geldig zijn");
}
std::string Renovation::getRoomIdentifier() const {
    ENSURE(!roomIdentifier.empty(),
           "Renovation room identifier mag niet leeg zijn");
    return roomIdentifier;
}

std::string Renovation::getStartDate() const {
    ENSURE(!startDate.empty(),
           "Renovation start date mag niet leeg zijn");
    return startDate;
}

std::string Renovation::getEndDate() const {
    ENSURE(!endDate.empty(),
           "Renovation end date mag niet leeg zijn");
    return endDate;
}
bool Renovation::isActiveOnDate(const std::string& date) const {
    REQUIRE(!date.empty(), "Date mag niet leeg zijn");

    bool active = (startDate <= date && date <= endDate);

    ENSURE(active == (startDate <= date && date <= endDate),
           "Renovation active status moet correct berekend zijn");

    return active;
}
