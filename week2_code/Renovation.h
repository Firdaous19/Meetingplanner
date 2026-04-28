//
// Created by firdi on 28/04/2026.
//

#ifndef PROJECTTITLE_RENOVATION_H
#define PROJECTTITLE_RENOVATION_H
#include <string>

class Renovation {
public:
    Renovation(const std::string& roomIdentifier,
               const std::string& startDate,
               const std::string& endDate);

    std::string getRoomIdentifier() const { return roomIdentifier; }
    std::string getStartDate() const { return startDate; }
    std::string getEndDate() const { return endDate; }

private:
    std::string roomIdentifier;
    std::string startDate;
    std::string endDate;
};
#endif //PROJECTTITLE_RENOVATION_H
