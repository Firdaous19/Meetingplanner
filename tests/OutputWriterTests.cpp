#include "gtest/gtest.h"
#include "week2_code/OutputWriter.h"
#include "week2_code/MeetingPlanner.h"
#include "week2_code/Room.h"
#include "week2_code/Meeting.h"
#include "week2_code/CateringProvider.h"
#include <fstream>
#include <sstream>

TEST(OutputWriterTest, WriteOutputCreatesFileWithExpectedSections) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room("M.G.023", "Room123", 10, "CDE", "CDE_R");
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");
    CateringProvider provider("CDE", 20.0f);

    meeting.setExternalsAllowed(true);
    meeting.setCatering(true);

    planner.addRoom(room);
    planner.addCateringProvider(provider);
    planner.addMeeting(meeting);
    planner.addParticipation("Meeting123", "Alice");

    ASSERT_TRUE(planner.checkConsistency());
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
    EXPECT_NE(content.find("CO2 emitted:"), std::string::npos);
    EXPECT_NE(content.find("Externals allowed"), std::string::npos);
    EXPECT_NE(content.find("Catering"), std::string::npos);
    EXPECT_NE(content.find("Room occupancy"), std::string::npos);

    EXPECT_TRUE(
            content.find("CO2 emitted: 140") != std::string::npos ||
            content.find("CO2 emitted: 140.0") != std::string::npos ||
            content.find("CO2 emitted: 140g") != std::string::npos
    );
}

TEST(OutputWriterTest, WriteOutputShowsNoConflictsWhenNoneExist) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

TEST(OutputWriterTest, EmptyFilenameIsRejected) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    OutputWriter writer;

    EXPECT_DEATH(
            writer.writeOutput("", planner),
            "Output filename mag niet leeg zijn"
    );
}