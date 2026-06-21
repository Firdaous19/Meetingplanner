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
    XMLParser();

    void setLoggingEnabled(bool enabled);

    /**
     * Geeft false terug bij syntactisch kapotte XML.
     */
    bool parse(const std::string& filename, MeetingPlanner& planner) const;

    /**
     * Fouten worden geschreven naar de meegegeven stream.
     */
    SuccessEnum parse(const char* inputFilename,
                      std::ostream& errStream,
                      MeetingPlanner& planner) const;

private:
    bool loggingEnabled;
};

#endif