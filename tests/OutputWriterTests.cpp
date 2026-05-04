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
    planner.addCateringProvider(CateringProvider("Campus_CDE", 300));
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
    planner.addCateringProvider(CateringProvider("Campus_CDE", 300));
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
    planner.addCateringProvider(CateringProvider("Campus_CDE", 300));
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

TEST(OutputWriterTest, WriteOutputShowsOnlineMeetingAndOccupancy) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room("Online backup room", "R1", 10, "Campus_CDE", "Building_A");
    Meeting meeting("Online Meeting", "M1", "R1", "2026-05-22");

    meeting.setOnline(true);
    meeting.setCO2Emission(0);
    meeting.setOccupancyPercentage(0);

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addCateringProvider(CateringProvider("Campus_CDE", 300));
    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    const std::string filename = "test_online_output.txt";
    writer.writeOutput(filename, planner);

    std::ifstream input(filename.c_str());
    ASSERT_TRUE(input.is_open());

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();

    EXPECT_NE(content.find("Location: online"), std::string::npos);
    EXPECT_NE(content.find("Room occupancy: 0%"), std::string::npos);
    EXPECT_NE(content.find("CO2 emitted: 0g"), std::string::npos);
}

TEST(OutputWriterTest, WriteOutputShowsCateringAndCO2Summary) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room("Vergaderzaal A", "A101", 10, "Campus_CDE", "Building_A");
    Meeting meeting("Lunch meeting", "M1", "A101", "2026-05-22");

    meeting.setCatering(true);
    meeting.setCO2Emission(300);

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addCateringProvider(CateringProvider("Campus_CDE", 300));
    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    const std::string filename = "test_catering_co2_output.txt";
    writer.writeOutput(filename, planner);

    std::ifstream input(filename.c_str());
    ASSERT_TRUE(input.is_open());

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();

    EXPECT_NE(content.find("- Catering"), std::string::npos);
    EXPECT_NE(content.find("--== CO2 summary ==--"), std::string::npos);
    EXPECT_NE(content.find("Total CO2 emitted"), std::string::npos);
}

// USE CASE 3.8 - OUTPUT OF CATERING COSTS

TEST(OutputWriterTest, WriteOutputShowsMeetingCateringCost) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room("Vergaderzaal A", "A101", 10, "CDE", "CDE_A");
    Meeting meeting("Lunch meeting", "M1", "A101", "2026-05-22");
    CateringProvider provider("CDE", 20.0f);

    meeting.setCatering(true);

    planner.addRoom(room);
    planner.addCateringProvider(provider);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M1", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    const std::string filename = "test_output_catering_cost.txt";
    writer.writeOutput(filename, planner);

    std::ifstream input(filename.c_str());
    ASSERT_TRUE(input.is_open());

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();

    EXPECT_NE(content.find("- Catering"), std::string::npos);
    EXPECT_NE(content.find("- Catering cost: EUR 21.18"), std::string::npos);
}

TEST(OutputWriterTest, WriteOutputShowsTotalCateringCostSummary) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room1("Vergaderzaal A", "A101", 10, "CDE", "CDE_A");
    Room room2("Vergaderzaal B", "B202", 10, "CDE", "CDE_B");
    Meeting meeting1("Lunch meeting 1", "M1", "A101", "2026-05-22");
    Meeting meeting2("Lunch meeting 2", "M2", "B202", "2026-05-22");
    CateringProvider provider("CDE", 20.0f);

    meeting1.setCatering(true);
    meeting2.setCatering(true);

    planner.addRoom(room1);
    planner.addRoom(room2);
    planner.addCateringProvider(provider);
    planner.addMeeting(meeting1);
    planner.addMeeting(meeting2);

    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M2", "Bob");
    planner.addParticipation("M2", "Charlie");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    const std::string filename = "test_output_total_catering_cost.txt";
    writer.writeOutput(filename, planner);

    std::ifstream input(filename.c_str());
    ASSERT_TRUE(input.is_open());

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();

    EXPECT_NE(content.find("- Total catering cost: EUR 31.77"), std::string::npos);
}

TEST(OutputWriterTest, WriteOutputShowsZeroTotalCateringCostWithoutCatering) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    OutputWriter writer;

    Room room("Vergaderzaal A", "A101", 10, "CDE", "CDE_A");
    Meeting meeting("Gewone meeting", "M1", "A101", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    const std::string filename = "test_output_zero_catering_cost.txt";
    writer.writeOutput(filename, planner);

    std::ifstream input(filename.c_str());
    ASSERT_TRUE(input.is_open());

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();

    EXPECT_NE(content.find("- No catering"), std::string::npos);
    EXPECT_NE(content.find("- Total catering cost: EUR 0.00"), std::string::npos);
}