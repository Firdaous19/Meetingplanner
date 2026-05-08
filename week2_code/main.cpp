#include <iostream>
#include "MeetingPlanner.h"
#include "XMLParser.h"
#include "OutputWriter.h"

int main() {
    const std::string inputFile = "../week2_code/test_laatste.xml";
    const std::string outputFile = "output.txt";

    MeetingPlanner planner;      // planner bewaart en verwerkt alle data van het systeem//
    XMLParser parser;   //parser leest input//
    OutputWriter writer;   //object writer schrijft het resultaat naar een tekstbestand//

    bool success = parser.parse(inputFile, planner);   //parser gaat de XMLbestand van de inputfile openen en lezen. maakt dan objecten aan en voegt die toe aan planner//

    if (!success) {
        std::cerr << "Inlezen van XML is mislukt." << std::endl;
        return 1;
    }

    if (!planner.checkConsistency()) {
        std::cerr << "Systeem is inconsistent." << std::endl;
        return 1;
    }

    planner.processMeetings();     //het object planner gaat de meetings verwerken; vb controleren of een room bezet/in renovatie is , occupancy/CO2/cateringkost berekenen, successfull meetings bijhouden//
    writer.writeOutput(outputFile, planner);   // object writer gaat wat in de planner staat(meetings,rroms,conflicts,..) overschrijven in de outputfile output.txt//

    std::cout << "Aantal rooms geladen: " << planner.getRooms().size() << std::endl;
    std::cout << "Aantal meetings geladen: " << planner.getMeetings().size() << std::endl;
    std::cout << "Aantal succesvolle meetings: "
              << planner.getSuccessfulMeetings().size() << std::endl;
    std::cout << "Outputbestand gemaakt: " << outputFile << std::endl;

    return 0;
}