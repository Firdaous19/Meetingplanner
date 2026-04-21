#include <iostream>
#include "MeetingPlanner.h"
#include "XMLParser.h"

int main() {
    MeetingPlanner planner;
    XMLParser parser;

    bool success = parser.parse("../week2_code/test_laatste.xml", planner);

    if (!success) {
        std::cerr << "Inlezen van XML is mislukt." << std::endl;
        return 1;
    }

    if (!planner.checkConsistency()) {
        std::cerr << "Systeem is inconsistent." << std::endl;
        return 1;
    }

    std::cout << "Aantal rooms geladen: " << planner.getRooms().size() << std::endl;
    std::cout << "Aantal meetings geladen: " << planner.getMeetings().size() << std::endl;
    std::cout << "Systeem is consistent." << std::endl;

    return 0;
}