#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include "MeetingPlanner.h"

class XMLParser {
public:
    XMLParser();

    void setLoggingEnabled(bool enabled);
    bool parse(const std::string& filename, MeetingPlanner& planner) const;

private:
    bool loggingEnabled;
};

#endif