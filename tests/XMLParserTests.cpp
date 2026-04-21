//
// Created by firdi on 21/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/XMLParser.h"
#include "week2_code/MeetingPlanner.h"

TEST(XMLParserTest, ParseValidFileLoadsRoomsAndMeetings) {
    MeetingPlanner planner;
    XMLParser parser;

    bool success = parser.parse("../week2_code/test.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_GT(planner.getRooms().size(), 0);
    EXPECT_GT(planner.getMeetings().size(), 0);
}

TEST(XMLParserTest, ParseFileWithUnknownMeetingParticipationStillSucceeds) {
    MeetingPlanner planner;
    XMLParser parser;

    bool success = parser.parse("../week2_code/test_fout.xml", planner);

    EXPECT_TRUE(success);
}

TEST(XMLParserTest, ParseDuplicateFileStillLoadsData) {
    MeetingPlanner planner;
    XMLParser parser;

    bool success = parser.parse("../week2_code/test_duplicate.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_GT(planner.getRooms().size(), 0);
}
TEST(XMLParserTest, ParseRoomWithMissingFieldDoesNotAddRoom) {
    MeetingPlanner planner;
    XMLParser parser;

    bool success = parser.parse("../week2_code/test_room_missing_field.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_EQ(planner.getRooms().size(), 0);
}

TEST(XMLParserTest, ParseRoomWithInvalidCapacityDoesNotAddRoom) {
    MeetingPlanner planner;
    XMLParser parser;

    bool success = parser.parse("../week2_code/test_room_invalid_capacity.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_EQ(planner.getRooms().size(), 0);
}

TEST(XMLParserTest, ParseMeetingWithMissingFieldDoesNotAddMeeting) {
    MeetingPlanner planner;
    XMLParser parser;

    bool success = parser.parse("../week2_code/test_meeting_missing_field.xml", planner);

    EXPECT_TRUE(success);
    EXPECT_EQ(planner.getMeetings().size(), 0);
}