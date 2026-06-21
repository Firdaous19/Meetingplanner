#include <fstream>
#include <iostream>
#include <string>

#include "MeetingPlanner.h"
#include "OutputWriter.h"
#include "XMLParser.h"

int main() {
    const std::string inputFile = "../week2_code/test_invalid_room.xml";
    const std::string outputFile = "output.txt";
    const std::string importErrorFile = "import_errors.txt";

    MeetingPlanner planner;
    XMLParser parser;
    OutputWriter writer;

    std::ofstream errorFile(
            importErrorFile.c_str(),
            std::ios::out | std::ios::trunc);

    if (!errorFile.is_open()) {
        std::cerr << "Kon " << importErrorFile
                  << " niet openen."
                  << std::endl;
        return 1;
    }

    const SuccessEnum importResult =
            parser.parse(inputFile.c_str(), errorFile, planner);

    errorFile.close();

    if (importResult == ImportAborted) {
        std::cerr << "XML-import afgebroken. Bekijk "
                  << importErrorFile
                  << "."
                  << std::endl;
        return 1;
    }

    if (importResult == PartialImport) {
        std::cerr << "XML-import bevat inhoudelijke fouten. Bekijk "
                  << importErrorFile
                  << "."
                  << std::endl;
        return 1;
    }

    if (!planner.checkConsistency()) {
        std::cerr << "Het geïmporteerde systeem is niet consistent."
                  << std::endl;
        return 1;
    }

    planner.processMeetings();
    writer.writeOutput(outputFile, planner);

    std::cout << "Verwerking geslaagd." << std::endl;
    std::cout << "Resultaat geschreven naar: "
              << outputFile
              << std::endl;

    return 0;
}