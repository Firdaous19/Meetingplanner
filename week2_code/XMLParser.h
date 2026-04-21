#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include "MeetingPlanner.h"

/**
 * Verantwoordelijk voor het inlezen van XML-bestanden
 * en het vullen van een MeetingPlanner met rooms,
 * meetings en participations.
 */
class XMLParser {
public:
    /**
     * Lees een XML-bestand in en vul het gegeven systeem.
     * @param filename Het pad naar het XML-bestand.
     * @param planner Het systeem dat gevuld moet worden.
     * @return true als het bestand gelezen kon worden, anders false.
     */
    bool parse(const std::string& filename, MeetingPlanner& planner) const;
};

#endif