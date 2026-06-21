#ifndef PROJECTTITLE_RENOVATION_H
#define PROJECTTITLE_RENOVATION_H

#include <string>

/**
 * Stelt een renovatieperiode van een specifieke room voor.
 * Tijdens deze periode kan de room niet gebruikt worden voor meetings.
 */
class Renovation {
public:
    /**
     * Constructor van een renovatie.
     * @param roomIdentifier De identifier van de room die gerenoveerd wordt.
     * @param startDate De startdatum van de renovatie.
     * @param endDate De einddatum van de renovatie.
     *
     * REQUIRE(!roomIdentifier.empty(), "Renovation room identifier mag niet leeg zijn");
     * REQUIRE(!startDate.empty(), "Renovation start date mag niet leeg zijn");
     * REQUIRE(!endDate.empty(), "Renovation end date mag niet leeg zijn");
     * REQUIRE(startDate <= endDate, "Renovation end date moet na start date of gelijk aan start date liggen");
     * ENSURE(this->roomIdentifier == roomIdentifier, "Room identifier correct opgeslagen");
     * ENSURE(this->startDate == startDate, "Start date correct opgeslagen");
     * ENSURE(this->endDate == endDate, "End date correct opgeslagen");
     * ENSURE(this->startDate <= this->endDate, "Start en end date moeten correct opgeslagen en geldig zijn");
     */
    Renovation(const std::string& roomIdentifier,
               const std::string& startDate,
               const std::string& endDate);

    /**
     * Geef de room identifier terug.
     * @return Identifier van de gerenoveerde room.
     */
    std::string getRoomIdentifier() const;

    /**
     * Geef de startdatum van de renovatie terug.
     * @return Startdatum in formaat YYYY-MM-DD.
     */
    std::string getStartDate() const;

    /**
     * Geef de einddatum van de renovatie terug.
     * @return Einddatum in formaat YYYY-MM-DD.
     */
    std::string getEndDate() const;

    /**
     * Controleer of de renovatie actief is op een specifieke datum.
     * @param date De te controleren datum.
     * @return true als de renovatie actief is op deze datum.
     *
     * REQUIRE(!date.empty(), "Date mag niet leeg zijn");
     * ENSURE(active == (startDate <= date && date <= endDate),
     *        "Renovation active status moet correct berekend zijn");
     */
    bool isActiveOnDate(const std::string& date) const;

private:
    std::string roomIdentifier;
    std::string startDate;
    std::string endDate;
};

#endif // PROJECTTITLE_RENOVATION_H
