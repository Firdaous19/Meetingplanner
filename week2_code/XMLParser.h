#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include "MeetingPlanner.h"

class XMLParser {
public:
    /**
     * Constructor van de XMLParser.
     */
    XMLParser();

    /**
     * Zet logging aan of uit.
     * @param enabled true om logging te activeren, false om logging uit te zetten.
     */
    void setLoggingEnabled(bool enabled);

    /**
     * Parse een XML-bestand en laad de inhoud in een MeetingPlanner.
     * @param filename De naam van het XML-bestand.
     * @param planner De planner waarin de gegevens geladen worden.
     * @return true als het bestand succesvol verwerkt werd, false bij fout openen of foutieve root.
     *
     * REQUIRE(!filename.empty(), "Bestandsnaam mag niet leeg zijn");
     */
    bool parse(const std::string& filename, MeetingPlanner& planner) const;

private:
    bool loggingEnabled;
};

#endif
