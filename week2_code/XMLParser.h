#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <iosfwd>
#include <string>

#include "MeetingPlanner.h"

/**
 * Resultaat van een XML-import.
 *
 * ImportAborted= XML is syntactisch kapot en TinyXML kan niet verder.
 * PartialImport= XML is syntactisch geldig, maar heeft inhoudelijke fouten.
 * Success= Alles werd correct geïmporteerd.
 */
enum SuccessEnum {
    ImportAborted,
    PartialImport,
    Success
};

class XMLParser {
public:
    /**
 * Constructor van de XMLParser.
 *
 * ENSURE(loggingEnabled, "Logging staat standaard aan");
 */
    XMLParser();

/**
 * Zet logging aan of uit.
 * @param enabled true als foutmeldingen naar std::cerr geschreven mogen worden.
 *
 * ENSURE(loggingEnabled == enabled, "Logging flag correct opgeslagen");
 */
    void setLoggingEnabled(bool enabled);



    /**
* Parse een XML-bestand en vul de planner.
* @param filename Naam van het XML-bestand.
* @param planner MeetingPlanner die gevuld wordt.
* @return true als import niet volledig werd afgebroken.
*
* REQUIRE(!filename.empty(), "Bestandsnaam mag niet leeg zijn");
*/
    bool parse(const std::string& filename, MeetingPlanner& planner) const;

    /**
 * Parse een XML-bestand en schrijf foutmeldingen naar een stream.
 * @param inputFilename Naam van het XML-bestand.
 * @param errStream Stream voor foutmeldingen.
 * @param planner MeetingPlanner die gevuld wordt.
 * @return ImportAborted, PartialImport of Success.
 *
 * REQUIRE(inputFilename != nullptr, "Bestandsnaam mag niet null zijn");
 * REQUIRE(inputFilename[0] != '\0', "Bestandsnaam mag niet leeg zijn");
 */
    SuccessEnum parse(const char* inputFilename,
                      std::ostream& errStream,
                      MeetingPlanner& planner) const;

private:
    bool loggingEnabled;
};

#endif