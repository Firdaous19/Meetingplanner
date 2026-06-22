#include <fstream>
#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "week2_code/MeetingPlanner.h"
#include "week2_code/XMLParser.h"

namespace {

const std::string INPUT_DIRECTORY = "../week2_code/";
const std::string ACTUAL_ERROR_FILE = "test_import_errors.txt";
const std::string FAILURE_REPORT_FILE = "Report_test_failure.txt";

bool reportHasFailures = false;

std::string inputPath(const std::string& filename) {
    return INPUT_DIRECTORY + filename;
}

bool fileExists(const std::string& filename) {
    std::ifstream input(filename.c_str());
    return input.good();
}

bool fileIsEmpty(const std::string& filename) {
    std::ifstream input(filename.c_str());

    if (!input.is_open()) {
        return false;
    }

    char character;
    return !input.get(character);
}

std::string readFile(const std::string& filename) {
    std::ifstream input(filename.c_str());
    std::stringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

void removeTrailingWhitespace(std::string& text) {
    while (!text.empty() &&
           (text[text.size() - 1] == '\n' ||
            text[text.size() - 1] == '\r' ||
            text[text.size() - 1] == ' ')) {
        text.erase(text.size() - 1);
    }
}

bool fileCompare(const std::string& expectedFile,
                 const std::string& actualFile) {
    if (!fileExists(expectedFile) || !fileExists(actualFile)) {
        return false;
    }

    std::string expected = readFile(expectedFile);
    std::string actual = readFile(actualFile);

    removeTrailingWhitespace(expected);
    removeTrailingWhitespace(actual);

    return expected == actual;
}

std::string resultToString(SuccessEnum result) {
    switch (result) {
        case ImportAborted:
            return "ImportAborted";

        case PartialImport:
            return "PartialImport";

        case Success:
            return "Success";
    }

    return "Unknown result";
}

std::string currentTestName() {
    const ::testing::TestInfo* testInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();

    if (testInfo == nullptr) {
        return "Unknown test";
    }

    return std::string(testInfo->test_suite_name()) +
           "." +
           testInfo->name();
}

void appendFailureReport(const std::string& xmlFilename,
                         const std::string& expectedResult,
                         const std::string& actualResult,
                         const std::string& expectedOutput,
                         const std::string& actualOutput) {
    reportHasFailures = true;

    std::ofstream report(
            FAILURE_REPORT_FILE.c_str(),
            std::ios::out | std::ios::app);

    if (!report.is_open()) {
        return;
    }

    report << "==================================================\n";
    report << "TEST: " << currentTestName() << "\n";
    report << "XML FILE: " << xmlFilename << "\n";
    report << "EXPECTED RESULT: " << expectedResult << "\n";
    report << "ACTUAL RESULT: " << actualResult << "\n\n";

    report << "EXPECTED OUTPUT:\n";
    report << expectedOutput << "\n\n";

    report << "ACTUAL OUTPUT:\n";
    report << actualOutput << "\n";
    report << "==================================================\n\n";
}

} // namespace

class XMLParserTest : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        reportHasFailures = false;

        std::ofstream report(
                FAILURE_REPORT_FILE.c_str(),
                std::ios::out | std::ios::trunc);

        if (report.is_open()) {
            report << "XMLParser test failure report\n";
            report << "=============================\n\n";
        }
    }

    static void TearDownTestSuite() {
        std::ofstream report(
                FAILURE_REPORT_FILE.c_str(),
                std::ios::out | std::ios::app);

        if (!report.is_open()) {
            return;
        }

        if (!reportHasFailures) {
            report << "Geen XMLParser-tests gefaald.\n";
        }
    }

protected:
    MeetingPlanner planner;
    XMLParser parser;

    void SetUp() override {
        planner.setLoggingEnabled(false);
        parser.setLoggingEnabled(false);
    }

    SuccessEnum importFile(const std::string& xmlFilename) {
        std::ofstream errorOutput(
                ACTUAL_ERROR_FILE.c_str(),
                std::ios::out | std::ios::trunc);

        if (!errorOutput.is_open()) {
            ADD_FAILURE() << "Kon " << ACTUAL_ERROR_FILE
                          << " niet openen.";
            return ImportAborted;
        }

        const std::string xmlPath = inputPath(xmlFilename);

        const SuccessEnum result =
                parser.parse(xmlPath.c_str(), errorOutput, planner);

        errorOutput.close();

        return result;
    }

    void expectSuccess(const std::string& xmlFilename) {
        const SuccessEnum result = importFile(xmlFilename);
        const std::string actualOutput = readFile(ACTUAL_ERROR_FILE);

        if (result != Success) {
            ADD_FAILURE()
                    << "Success verwacht voor "
                    << xmlFilename
                    << ", maar kreeg "
                    << resultToString(result)
                    << ".\nWerkelijke fouttekst:\n"
                    << actualOutput;

            appendFailureReport(
                    xmlFilename,
                    "Success",
                    resultToString(result),
                    "Geen foutmeldingen verwacht.",
                    actualOutput);

            return;
        }

        if (!fileIsEmpty(ACTUAL_ERROR_FILE)) {
            ADD_FAILURE()
                    << "Geen foutmeldingen verwacht voor "
                    << xmlFilename
                    << ".\nWerkelijke fouttekst:\n"
                    << actualOutput;

            appendFailureReport(
                    xmlFilename,
                    "Success",
                    "Success",
                    "Geen foutmeldingen verwacht.",
                    actualOutput);
        }
    }

    void expectPartialImport(const std::string& xmlFilename,
                             const std::string& expectedErrorFilename) {
        const SuccessEnum result = importFile(xmlFilename);
        const std::string expectedPath = inputPath(expectedErrorFilename);
        const std::string expectedOutput = readFile(expectedPath);
        const std::string actualOutput = readFile(ACTUAL_ERROR_FILE);

        if (result != PartialImport) {
            ADD_FAILURE()
                    << "PartialImport verwacht voor "
                    << xmlFilename
                    << ", maar kreeg "
                    << resultToString(result)
                    << ".\n\nVerwacht:\n"
                    << expectedOutput
                    << "\nWerkelijk:\n"
                    << actualOutput;

            appendFailureReport(
                    xmlFilename,
                    "PartialImport",
                    resultToString(result),
                    expectedOutput,
                    actualOutput);

            return;
        }

        if (!fileCompare(expectedPath, ACTUAL_ERROR_FILE)) {
            ADD_FAILURE()
                    << "Fouttekst verschilt voor "
                    << xmlFilename
                    << ".\n\nVerwacht:\n"
                    << expectedOutput
                    << "\nWerkelijk:\n"
                    << actualOutput;

            appendFailureReport(
                    xmlFilename,
                    "PartialImport",
                    "PartialImport",
                    expectedOutput,
                    actualOutput);
        }
    }

    void expectImportAborted(const std::string& xmlFilename,
                             const std::string& expectedErrorFilename) {
        const SuccessEnum result = importFile(xmlFilename);
        const std::string expectedPath = inputPath(expectedErrorFilename);
        const std::string expectedOutput = readFile(expectedPath);
        const std::string actualOutput = readFile(ACTUAL_ERROR_FILE);

        if (result != ImportAborted) {
            ADD_FAILURE()
                    << "ImportAborted verwacht voor "
                    << xmlFilename
                    << ", maar kreeg "
                    << resultToString(result)
                    << ".\n\nVerwacht:\n"
                    << expectedOutput
                    << "\nWerkelijk:\n"
                    << actualOutput;

            appendFailureReport(
                    xmlFilename,
                    "ImportAborted",
                    resultToString(result),
                    expectedOutput,
                    actualOutput);

            return;
        }

        if (!fileCompare(expectedPath, ACTUAL_ERROR_FILE)) {
            ADD_FAILURE()
                    << "Fouttekst verschilt voor "
                    << xmlFilename
                    << ".\n\nVerwacht:\n"
                    << expectedOutput
                    << "\nWerkelijk:\n"
                    << actualOutput;

            appendFailureReport(
                    xmlFilename,
                    "ImportAborted",
                    "ImportAborted",
                    expectedOutput,
                    actualOutput);
        }
    }
};

/* =========================================================
 * GELDIGE BASIS-IMPORTS
 * ========================================================= */

TEST_F(XMLParserTest, ParseValidFileLoadsRoomsAndMeetings) {
    expectSuccess("test.xml");

    EXPECT_GT(planner.getRooms().size(), 0);
    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST_F(XMLParserTest, ParseDuplicateFileStillLoadsData) {
    expectSuccess("test_duplicate.xml");

    EXPECT_GT(planner.getRooms().size(), 0);
}

TEST_F(XMLParserTest, ParseBuildingsFileLoadsCampusesAndBuildings) {
    expectSuccess("test_buildings.xml");

    ASSERT_EQ(1, planner.getCampuses().size());
    ASSERT_EQ(1, planner.getBuildings().size());

    EXPECT_EQ("CDE", planner.getCampuses()[0].getIdentifier());
    EXPECT_EQ("CDE_R", planner.getBuildings()[0].getIdentifier());
    EXPECT_EQ("CDE", planner.getBuildings()[0].getCampusIdentifier());
}

TEST_F(XMLParserTest, ParseRenovationFileLoadsRenovations) {
    expectSuccess("test_renovation.xml");

    ASSERT_EQ(1, planner.getRenovations().size());

    EXPECT_EQ("A101",
              planner.getRenovations()[0].getRoomIdentifier());
    EXPECT_EQ("2026-04-01",
              planner.getRenovations()[0].getStartDate());
    EXPECT_EQ("2026-06-01",
              planner.getRenovations()[0].getEndDate());
}

TEST_F(XMLParserTest, ParseCateringFileLoadsProviderAndMeetingFlag) {
    expectSuccess("test_catering.xml");

    ASSERT_EQ(1, planner.getCateringProviders().size());

    EXPECT_EQ("Campus_CDE",
              planner.getCateringProviders()[0].getCampusIdentifier());
    EXPECT_FLOAT_EQ(20.0f,
                    planner.getCateringProviders()[0].getCO2());

    ASSERT_EQ(1, planner.getMeetings().size());

    EXPECT_TRUE(planner.getMeetings()[0].hasCatering());
    EXPECT_FALSE(planner.getMeetings()[0].isOnline());
}

TEST_F(XMLParserTest, ParseOnlineMeetingWithoutRoomSucceeds) {
    expectSuccess("test_online_meeting.xml");

    ASSERT_EQ(1, planner.getMeetings().size());

    EXPECT_TRUE(planner.getMeetings()[0].isOnline());
    EXPECT_FALSE(planner.getMeetings()[0].hasCatering());
    EXPECT_EQ("", planner.getMeetings()[0].getRoomIdentifier());
}

TEST_F(XMLParserTest, ParseMeetingWithExternalsCorrectly) {
    expectSuccess("test_meeting_hour_externals.xml");

    ASSERT_EQ(1, planner.getMeetings().size());

    EXPECT_EQ("M1", planner.getMeetings()[0].getIdentifier());
    EXPECT_FALSE(planner.getMeetings()[0].isOnline());
    EXPECT_TRUE(planner.getMeetings()[0].areExternalsAllowed());
}

TEST_F(XMLParserTest, ParseExternalParticipationCorrectly) {
    expectSuccess("test_external_participation.xml");

    ASSERT_EQ(1, planner.getMeetings().size());

    const Meeting& meeting = planner.getMeetings()[0];

    EXPECT_TRUE(meeting.areExternalsAllowed());
    EXPECT_EQ(1, meeting.getExternalParticipantCount());
    EXPECT_GE(meeting.getParticipants().size(), 1);
}

/* =========================================================
 * EXTRA GELDIGE XML-BESTANDEN
 * ========================================================= */

TEST_F(XMLParserTest, CO2OnlineMeetingFileCanBeImported) {
    expectSuccess("test_CO2_online_meeting.xml");

    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST_F(XMLParserTest, PhysicalMeetingWithoutCateringFileCanBeImported) {
    expectSuccess("test_CO2_PhysicalMeeting_NoCatering.xml");

    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST_F(XMLParserTest, PhysicalMeetingWithCateringFileCanBeImported) {
    expectSuccess("test_CO2_physicalMeeting_with_catering.xml");

    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST_F(XMLParserTest, DemoFullFileCanBeImported) {
    expectSuccess("test_demo_full.xml");

    EXPECT_GT(planner.getRooms().size(), 0);
    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST_F(XMLParserTest, ExternalsFileImportsCorrectly) {
    expectSuccess("test_externals.xml");

    EXPECT_GT(planner.getRooms().size(), 0);
    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST_F(XMLParserTest, FoutFileCanBeImported) {
    expectSuccess("test_fout.xml");
}

TEST_F(XMLParserTest, LaatsteFileCanBeImported) {
    expectSuccess("test_laatste.xml");

    EXPECT_GT(planner.getRooms().size(), 0);
    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST_F(XMLParserTest, RoomFoutFileCanBeImported) {
    expectSuccess("test_room_fout.xml");
}

TEST_F(XMLParserTest, DuplicateCateringProvidersCauseConsistencyFailure) {
    expectSuccess("test_two_catering_providers_same_campus.xml");

    EXPECT_EQ(2, planner.getCateringProviders().size());
    EXPECT_FALSE(planner.checkConsistency());
}

/* =========================================================
 * GELDIGE IMPORT + VERWERKING
 * ========================================================= */

TEST_F(XMLParserTest, CateringMeetingCanBeProcessedAndTracksCO2) {
    expectSuccess("test_catering.xml");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(1, planner.getSuccessfulMeetings().size());

    EXPECT_FLOAT_EQ(
            140.0f,
            planner.getSuccessfulMeetings()[0].getCO2Emission());

    EXPECT_FLOAT_EQ(140.0f, planner.getTotalCO2Emission());
}

TEST_F(XMLParserTest, OnlineMeetingCanBeProcessedAndTracksCO2) {
    expectSuccess("test_online_meeting.xml");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(1, planner.getSuccessfulMeetings().size());

    EXPECT_FLOAT_EQ(
            60.0f,
            planner.getSuccessfulMeetings()[0].getCO2Emission());

    EXPECT_FLOAT_EQ(60.0f, planner.getTotalCO2Emission());
}

/* =========================================================
 * PARTIAL IMPORTS MET EXACTE FOUTTEKST
 * ========================================================= */

TEST_F(XMLParserTest, InvalidRoomProducesExpectedPartialImportError) {
    expectPartialImport(
            "test_invalid_room.xml",
            "Error_invalid_room.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}

TEST_F(XMLParserTest, RoomMissingFieldProducesExpectedPartialImportError) {
    expectPartialImport(
            "test_room_missing_field.xml",
            "Error_room_missing_field.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}

TEST_F(XMLParserTest, RoomInvalidCapacityProducesExpectedPartialImportError) {
    expectPartialImport(
            "test_room_invalid_capacity.xml",
            "Error_room_invalid_capacity.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}

TEST_F(XMLParserTest, MeetingMissingFieldProducesExpectedPartialImportError) {
    expectPartialImport(
            "test_meeting_missing_field.xml",
            "Error_meeting_missing_field.txt");

    EXPECT_EQ(0, planner.getMeetings().size());
}

TEST_F(XMLParserTest, MultipleInputErrorsContinueWithValidElements) {
    expectPartialImport(
            "test_multiple_input_errors.xml",
            "Error_multiple_input_errors.txt");

    ASSERT_EQ(1, planner.getRooms().size());
    EXPECT_EQ("GoodRoom", planner.getRooms()[0].getIdentifier());

    ASSERT_EQ(1, planner.getMeetings().size());
    EXPECT_EQ("GoodMeeting",
              planner.getMeetings()[0].getIdentifier());

    ASSERT_EQ(1,
              planner.getMeetings()[0].getParticipants().size());

    EXPECT_EQ("Alice",
              planner.getMeetings()[0].getParticipants()[0]);
}

TEST_F(XMLParserTest, WrongSystemRootProducesExpectedPartialImportError) {
    expectPartialImport(
            "test_no_system_root.xml",
            "Error_no_system_root.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}

TEST_F(XMLParserTest, OnlineMeetingWithCateringProducesExpectedErrors) {
    expectPartialImport(
            "test_ongeldige_online_meeting_catering.xml",
            "Error_ongeldige_online_meeting_catering.txt");

    EXPECT_EQ(0, planner.getMeetings().size());
}

/* =========================================================
 * IMPORT ABORTED: XML-SYNTAXFOUTEN
 * ========================================================= */

TEST_F(XMLParserTest, SyntaxErrorWrongEndTagAbortsImport) {
    expectImportAborted(
            "test_xmlsyntax_foute_end_tag.xml",
            "Error_xmlsyntax_foute_end_tag.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}

TEST_F(XMLParserTest, SyntaxErrorEmptyDocumentAbortsImport) {
    expectImportAborted(
            "test_xmlsyntax_leeg_bestand.xml",
            "Error_xmlsyntax_leeg_bestand.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}

TEST_F(XMLParserTest, SyntaxErrorInvalidEmptyTagAbortsImport) {
    expectImportAborted(
            "test_xmlsyntax_lege_tag.xml",
            "Error_xmlsyntax_lege_tag.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}

TEST_F(XMLParserTest, SyntaxErrorReadingAttributesAbortsImport) {
    expectImportAborted(
            "test_xmlsyntax_reading_attributes.xml",
            "Error_xmlsyntax_reading_attributes.txt");

    EXPECT_EQ(0, planner.getRooms().size());
}