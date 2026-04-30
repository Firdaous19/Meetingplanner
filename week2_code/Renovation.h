//
// Created by firdi on 28/04/2026.
//

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
     */
    Renovation(const std::string& roomIdentifier,
               const std::string& startDate,
               const std::string& endDate);

    /**
     * Geef de room identifier terug.
     * @return Identifier van de gerenoveerde room.
     */
    std::string getRoomIdentifier() const { return roomIdentifier; }

    /**
     * Geef de startdatum van de renovatie terug.
     * @return Startdatum in formaat YYYY-MM-DD.
     */
    std::string getStartDate() const { return startDate; }

    /**
     * Geef de einddatum van de renovatie terug.
     * @return Einddatum in formaat YYYY-MM-DD.
     */
    std::string getEndDate() const { return endDate; }

    /**
     * Controleer of de renovatie actief is op een specifieke datum.
     * @param date De te controleren datum.
     * @return true als de renovatie actief is op deze datum.
     */
    bool isActiveOnDate(const std::string& date) const;

private:
    std::string roomIdentifier;
    std::string startDate;
    std::string endDate;
};

#endif // PROJECTTITLE_RENOVATION_H