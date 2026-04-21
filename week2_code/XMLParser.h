//
// Created by firdi on 21/04/2026.
//

#ifndef PROJECTTITLE_XMLPARSER_H
#define PROJECTTITLE_XMLPARSER_H

#include <string>
#include "MeetingPlanner.h"

class XMLParser {
public:
    bool parse(const std::string& filename, MeetingPlanner& planner) const;
};


#endif //PROJECTTITLE_XMLPARSER_H
