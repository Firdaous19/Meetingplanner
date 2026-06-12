//
// Created by firdi on 21/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/XMLParser.h"
#include "week2_code/MeetingPlanner.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

namespace {
    std::string readFile(const std::string& filename) {
        std::ifstream input(filename.c_str());
        std::stringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    bool fileCompare(const std::string& expectedFile,
                     const std::string& actualFile) {
        return readFile(expectedFile) == readFile(actualFile);
    }
}
TEST(XMLParserTest, ParseValidFileLoadsRoomsAndMeetings) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_GT(planner.getRooms().size(), 0);
    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST(XMLParserTest, ParseFileWithUnknownMeetingParticipationStillSucceeds) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_fout.xml", planner);

    EXPECT_TRUE(success);
}

TEST(XMLParserTest, ParseDuplicateFileStillLoadsData) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_duplicate.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_GT(planner.getRooms().size(), 0);
}

TEST(XMLParserTest, ParseRoomWithMissingFieldDoesNotAddRoom) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_room_missing_field.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_EQ(planner.getRooms().size(), 0);
}

TEST(XMLParserTest, ParseRoomWithInvalidCapacityDoesNotAddRoom) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_room_invalid_capacity.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_EQ(planner.getRooms().size(), 0);
}

TEST(XMLParserTest, ParseMeetingWithMissingFieldDoesNotAddMeeting) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_meeting_missing_field.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_EQ(planner.getMeetings().size(), 0);
}

TEST(XMLParserTest, ParseBuildingsFileLoadsCampusesAndBuildings) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_buildings.xml", planner);

    EXPECT_TRUE(success);
    ASSERT_EQ(planner.getCampuses().size(), 1);
    ASSERT_EQ(planner.getBuildings().size(), 1);
    EXPECT_EQ(planner.getCampuses()[0].getIdentifier(), "CDE");
    EXPECT_EQ(planner.getBuildings()[0].getIdentifier(), "CDE_R");
    EXPECT_EQ(planner.getBuildings()[0].getCampusIdentifier(), "CDE");
}

TEST(XMLParserTest, ParseRenovationFileLoadsRenovations) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_renovation.xml", planner);

    EXPECT_TRUE(success);
    ASSERT_EQ(planner.getRenovations().size(), 1);
    EXPECT_EQ(planner.getRenovations()[0].getRoomIdentifier(), "A101");
    EXPECT_EQ(planner.getRenovations()[0].getStartDate(), "2026-04-01");
    EXPECT_EQ(planner.getRenovations()[0].getEndDate(), "2026-06-01");
}

TEST(XMLParserTest, ParseCateringFileLoadsCateringProviderAndMeetingFlag) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_catering.xml", planner);

    EXPECT_TRUE(success);
    ASSERT_EQ(planner.getCateringProviders().size(), 1);
    EXPECT_EQ(planner.getCateringProviders()[0].getCampusIdentifier(), "Campus_CDE");
    EXPECT_FLOAT_EQ(planner.getCateringProviders()[0].getCO2(), 20.0f);

    ASSERT_EQ(planner.getMeetings().size(), 1);
    EXPECT_TRUE(planner.getMeetings()[0].hasCatering());
    EXPECT_FALSE(planner.getMeetings()[0].isOnline());
}

TEST(XMLParserTest, ParseFileWithMultipleInputErrorsContinuesReadingValidElements) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_multiple_input_errors.xml", planner);

    EXPECT_TRUE(success);

    ASSERT_EQ(planner.getRooms().size(), 1);
    EXPECT_EQ(planner.getRooms()[0].getIdentifier(), "GoodRoom");

    ASSERT_EQ(planner.getMeetings().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getIdentifier(), "GoodMeeting");

    ASSERT_EQ(planner.getMeetings()[0].getParticipants().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getParticipants()[0], "Alice");
}

TEST(XMLParserTest, ParseNonExistingFileReturnsFalse) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/does_not_exist.xml", planner);

    EXPECT_FALSE(success);
}

TEST(XMLParserTest, ParseFileWithoutSystemRootReturnsFalse) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_no_system_root.xml", planner);

    EXPECT_FALSE(success);
    EXPECT_EQ(planner.getRooms().size(), 0);
}

   //Use Case 3.4//

TEST(XMLParserTest, ParseOnlineMeetingWithoutRoomSucceeds) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_online_meeting.xml", planner);

    EXPECT_TRUE(success);
    ASSERT_EQ(planner.getMeetings().size(), 1);
    EXPECT_TRUE(planner.getMeetings()[0].isOnline());
    EXPECT_FALSE(planner.getMeetings()[0].hasCatering());
    EXPECT_EQ(planner.getMeetings()[0].getRoomIdentifier(), "");
}

TEST(XMLParserTest, ParseOnlineMeetingWithCateringDoesNotAddMeeting) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_ongeldige_online_meeting_catering.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_EQ(planner.getMeetings().size(), 0);
}

// Use Case 3.5 //

TEST(XMLParserTest, ParseCateringFileCanBeProcessedAndTracksCO2) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_catering.xml", planner);

    ASSERT_TRUE(success);
    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_FLOAT_EQ(planner.getSuccessfulMeetings()[0].getCO2Emission(), 140.0f);
    EXPECT_FLOAT_EQ(planner.getTotalCO2Emission(), 140.0f);
}

TEST(XMLParserTest, ParseOnlineMeetingCanBeProcessedAndTracksOnlineCO2) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_online_meeting.xml", planner);

    ASSERT_TRUE(success);
    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_FLOAT_EQ(planner.getSuccessfulMeetings()[0].getCO2Emission(), 60.0f);
    EXPECT_FLOAT_EQ(planner.getTotalCO2Emission(), 60.0f);
}
TEST(XMLParserTest, ParseMeetingHourAndExternalsCorrectly) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_meeting_hour_externals.xml", planner);

    EXPECT_TRUE(success);

    ASSERT_EQ(planner.getMeetings().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getIdentifier(), "M1");
    EXPECT_FALSE(planner.getMeetings()[0].isOnline());
    EXPECT_TRUE(planner.getMeetings()[0].areExternalsAllowed());
}
TEST(XMLParserTest, ParseExternalParticipationCorrectly) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    XMLParser parser;
    parser.setLoggingEnabled(false);

    bool success = parser.parse("../week2_code/test_external_participation.xml", planner);

    EXPECT_TRUE(success);

    ASSERT_EQ(planner.getMeetings().size(), 1);
    ASSERT_EQ(planner.getMeetings()[0].getParticipants().size(), 1);

    EXPECT_EQ(planner.getMeetings()[0].getParticipants()[0], "External User");
    EXPECT_TRUE(planner.getMeetings()[0].isParticipantExternal(0));
    EXPECT_EQ(planner.getMeetings()[0].getExternalParticipantCount(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getInternalParticipantCount(), 0);
}
TEST(XMLParserTest, FileCompareHelperDetectsEqualFiles) {
    std::ofstream expected("expected_file_compare_test.txt");
    expected << "Fout in ROOM: ontbrekende velden.\n";
    expected.close();

    std::ofstream actual("actual_file_compare_test.txt");
    actual << "Fout in ROOM: ontbrekende velden.\n";
    actual.close();

    EXPECT_TRUE(fileCompare("expected_file_compare_test.txt",
                            "actual_file_compare_test.txt"));

    std::remove("expected_file_compare_test.txt");
    std::remove("actual_file_compare_test.txt");
}