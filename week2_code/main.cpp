#include <iostream>
#include "MeetingPlanner.h"
#include "XMLParser.h"
#include "OutputWriter.h"

int main() {
    MeetingPlanner planner;
    XMLParser parser;
    OutputWriter writer;

    bool success = parser.parse("../week2_code/test_laatste.xml", planner);

    if (!success) {
        std::cerr << "Inlezen van XML is mislukt." << std::endl;
        return 1;
    }

    if (!planner.checkConsistency()) {
        std::cerr << "Systeem is inconsistent." << std::endl;
        return 1;
    }

    planner.processMeetings();
    writer.writeOutput("output.txt", planner);

    std::cout << "Aantal rooms geladen: " << planner.getRooms().size() << std::endl;
    std::cout << "Aantal meetings geladen: " << planner.getMeetings().size() << std::endl;
    std::cout << "Aantal succesvolle meetings: "
              << planner.getSuccessfulMeetings().size() << std::endl;
    std::cout << "Outputbestand gemaakt: output.txt" << std::endl;

    return 0;
}