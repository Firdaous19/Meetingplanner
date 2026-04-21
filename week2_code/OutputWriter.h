//
// Created by firdi on 21/04/2026.
//

#ifndef PROJECTTITLE_OUTPUTWRITER_H
#define PROJECTTITLE_OUTPUTWRITER_H

#include <string>
#include "MeetingPlanner.h"

class OutputWriter {
public:
    void writeOutput(const std::string& filename, const MeetingPlanner& planner) const;

private:
    std::string formatDate(const std::string& isoDate) const;
};
#endif //PROJECTTITLE_OUTPUTWRITER_H
