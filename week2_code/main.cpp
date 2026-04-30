#include <iostream>
#include "MeetingPlanner.h"
#include "XMLParser.h"
#include "OutputWriter.h"

int main() {
    const std::string inputFile = "../week2_code/test_laatste.xml";
    const std::string outputFile = "output.txt";

    MeetingPlanner planner;
    XMLParser parser;
    OutputWriter writer;

    bool success = parser.parse(inputFile, planner);

    if (!success) {
        std::cerr << "Inlezen van XML is mislukt." << std::endl;
        return 1;
    }

    if (!planner.checkConsistency()) {
        std::cerr << "Systeem is inconsistent." << std::endl;
        return 1;
    }

    planner.processMeetings();
    writer.writeOutput(outputFile, planner);

    std::cout << "Aantal rooms geladen: " << planner.getRooms().size() << std::endl;
    std::cout << "Aantal meetings geladen: " << planner.getMeetings().size() << std::endl;
    std::cout << "Aantal succesvolle meetings: "
              << planner.getSuccessfulMeetings().size() << std::endl;
    std::cout << "Outputbestand gemaakt: " << outputFile << std::endl;

    return 0;
}