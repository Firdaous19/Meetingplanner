#include "gtest/gtest.h"
#include "week2_code/OutputWriter.h"
#include "week2_code/MeetingPlanner.h"
#include "week2_code/Room.h"
#include "week2_code/Meeting.h"
#include <fstream>
#include <sstream>

TEST(OutputWriterTest, WriteOutputCreatesFileWithExpectedSections) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room("M.G.023", "Room123", 10);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    meeting.setExternalsAllowed(true);
    meeting.setCatering(true);
    meeting.setCO2Emission(560);

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

    EXPECT_NE(content.find("## ==== [SYSTEM STATUS] ==== ##"), std::string::npos);
    EXPECT_NE(content.find("--== Meetings ==--"), std::string::npos);
    EXPECT_NE(content.find("--== Rooms ==--"), std::string::npos);
    EXPECT_NE(content.find("--== CO2 summary ==--"), std::string::npos);
    EXPECT_NE(content.find("CO2 emitted: 560g"), std::string::npos);
    EXPECT_NE(content.find("Externals allowed"), std::string::npos);
    EXPECT_NE(content.find("Catering"), std::string::npos);
    EXPECT_NE(content.find("Room occupancy"), std::string::npos);
}
TEST(OutputWriterTest, WriteOutputShowsNoConflictsWhenNoneExist) {
MeetingPlanner planner;
planner.setLoggingEnabled(false);
OutputWriter writer;

Room room("Vergaderzaal A", "A101", 5);
Meeting meeting("Meeting", "M1", "A101", "2026-03-20");

planner.addRoom(room);
planner.addMeeting(meeting);
planner.addParticipation("M1", "Alice");

ASSERT_TRUE(planner.checkConsistency());

planner.processMeetings();

const std::string filename = "test_output_no_conflicts.txt";
writer.writeOutput(filename, planner);

std::ifstream input(filename.c_str());
ASSERT_TRUE(input.is_open());

std::stringstream buffer;
buffer << input.rdbuf();
std::string content = buffer.str();

EXPECT_NE(content.find("No conflicts"), std::string::npos);
}
TEST(OutputWriterTest, WriteOutputShowsConflictsWhenMeetingFails) {
MeetingPlanner planner;
planner.setLoggingEnabled(false);
OutputWriter writer;

Room room("Vergaderzaal A", "A101", 5);
Meeting meeting1("Meeting 1", "M1", "A101", "2026-03-20");
Meeting meeting2("Meeting 2", "M2", "A101", "2026-03-20");

planner.addRoom(room);
planner.addMeeting(meeting1);
planner.addMeeting(meeting2);
planner.addParticipation("M1", "Alice");
planner.addParticipation("M2", "Bob");

ASSERT_TRUE(planner.checkConsistency());

planner.processMeetings();

const std::string filename = "test_output_with_conflicts.txt";
writer.writeOutput(filename, planner);

std::ifstream input(filename.c_str());
ASSERT_TRUE(input.is_open());

std::stringstream buffer;
buffer << input.rdbuf();
std::string content = buffer.str();

EXPECT_NE(content.find("--== Conflicts ==--"), std::string::npos);
EXPECT_NE(content.find("geannuleerd"), std::string::npos);
EXPECT_NE(content.find("is al bezet"), std::string::npos);
}