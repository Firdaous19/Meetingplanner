//
// Created by firdi on 21/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/OutputWriter.h"
#include "week2_code/MeetingPlanner.h"
#include "week2_code/Room.h"
#include "week2_code/Meeting.h"
#include <fstream>
#include <sstream>

TEST(OutputWriterTest, WriteOutputCreatesFileWithExpectedSections) {
    MeetingPlanner planner;
    OutputWriter writer;

    Room room("M.G.023", "Room123", 10);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("Meeting123", "Alice");
    planner.checkConsistency();
    planner.processMeetings();

    const std::string filename = "test_output.txt";
    writer.writeOutput(filename, planner);

    std::ifstream input(filename.c_str());
    ASSERT_TRUE(input.is_open());

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();

    EXPECT_NE(content.find("Future meetings:"), std::string::npos);
    EXPECT_NE(content.find("Conflicts:"), std::string::npos);
    EXPECT_NE(content.find("Rooms:"), std::string::npos);
}