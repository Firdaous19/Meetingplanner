#ifndef OUTPUTWRITER_H
#define OUTPUTWRITER_H

#include <string>
#include "MeetingPlanner.h"

/**
 * Verantwoordelijk voor het schrijven van een tekstueel
 * outputbestand op basis van de inhoud van een MeetingPlanner.
 */
class OutputWriter {
public:
    /**
     * Schrijf de output van het systeem naar een bestand.
     * @param filename De naam van het outputbestand.
     * @param planner Het systeem waarvan de output geschreven wordt.
     */
    void writeOutput(const std::string& filename, const MeetingPlanner& planner) const;

private:
    /**
     * Zet een datum in ISO-formaat om naar DD/MM/YYYY.
     * @param isoDate Datum in formaat YYYY-MM-DD.
     * @return Geformatteerde datum.
     */
    std::string formatDate(const std::string& isoDate) const;
};

#endif