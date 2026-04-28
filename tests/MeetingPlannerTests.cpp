//
// Created by firdi on 21/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/MeetingPlanner.h"
#include "week2_code/Room.h"
#include "week2_code/Meeting.h"
#include "week2_code/Campus.h"
#include "week2_code/Building.h"

TEST(MeetingPlannerTest, AddRoomIncreasesRoomCount) {
    MeetingPlanner planner;
    Room room("M.G.023", "Room123", 10);

    planner.addRoom(room);

    EXPECT_EQ(planner.getRooms().size(), 1);
    EXPECT_EQ(planner.getRooms()[0].getIdentifier(), "Room123");
}

TEST(MeetingPlannerTest, AddMeetingIncreasesMeetingCount) {
    MeetingPlanner planner;
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addMeeting(meeting);

    EXPECT_EQ(planner.getMeetings().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getIdentifier(), "Meeting123");
}

TEST(MeetingPlannerTest, AddParticipationToExistingMeetingWorks) {
    MeetingPlanner planner;
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");
    planner.addMeeting(meeting);

    bool result = planner.addParticipation("Meeting123", "Alice");

    EXPECT_TRUE(result);
    EXPECT_EQ(planner.getMeetings()[0].getParticipants().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getParticipants()[0], "Alice");
}

TEST(MeetingPlannerTest, AddParticipationToUnknownMeetingFails) {
    MeetingPlanner planner;

    bool result = planner.addParticipation("BestaatNiet", "Alice");

    EXPECT_FALSE(result);
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsTrueForValidSystem) {
    MeetingPlanner planner;
    Room room("M.G.023", "Room123", 10);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("Meeting123", "Alice");

    EXPECT_TRUE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForDuplicateRoomIdentifiers) {
    MeetingPlanner planner;
    Room room1("M.G.023", "Room123", 10);
    Room room2("M.G.024", "Room123", 15);

    planner.addRoom(room1);
    planner.addRoom(room2);

    EXPECT_FALSE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForUnknownRoomReference) {
    MeetingPlanner planner;
    Meeting meeting("Weekly meeting", "Meeting123", "BestaatNiet", "2026-05-22");

    planner.addMeeting(meeting);

    EXPECT_FALSE(planner.checkConsistency());
}
TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForDuplicateMeetingIdentifiers) {
    MeetingPlanner planner;
    Room room("M.G.023", "Room123", 10);
    Meeting meeting1("Weekly meeting", "Meeting123", "Room123", "2026-05-22");
    Meeting meeting2("Another meeting", "Meeting123", "Room123", "2026-05-23");

    planner.addRoom(room);
    planner.addMeeting(meeting1);
    planner.addMeeting(meeting2);

    EXPECT_FALSE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseWhenRoomCapacityTooSmall) {
    MeetingPlanner planner;
    Room room("M.G.023", "Room123", 1);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("Meeting123", "Alice");
    planner.addParticipation("Meeting123", "Bob");

    EXPECT_FALSE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, ProcessMeetingsAddsSuccessfulMeeting) {
    MeetingPlanner planner;
    Room room("M.G.023", "Room123", 10);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("Meeting123", "Alice");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getIdentifier(), "Meeting123");
}

TEST(MeetingPlannerTest, ProcessMeetingsWithSameRoomCreatesConflict) {
    MeetingPlanner planner;
    Room room("M.G.023", "Room123", 10);
    Meeting meeting1("Weekly meeting", "Meeting123", "Room123", "2026-05-22");
    Meeting meeting2("Another meeting", "Meeting456", "Room123", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting1);
    planner.addMeeting(meeting2);
    planner.addParticipation("Meeting123", "Alice");
    planner.addParticipation("Meeting456", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_FALSE(planner.getConflicts().empty());
}
TEST(MeetingPlannerTest, ProcessingSingleMeetingSucceedsWhenRoomIsFree) {
    MeetingPlanner planner;
    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting("Eerste meeting", "M1", "A101", "2026-03-20");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_TRUE(planner.getRooms()[0].isOccupied());
    EXPECT_TRUE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, ProcessingSingleMeetingFailsWhenRoomAlreadyOccupied) {
    MeetingPlanner planner;
    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting1("Eerste meeting", "M1", "A101", "2026-03-20");
    Meeting meeting2("Tweede meeting", "M2", "A101", "2026-03-20");

    planner.addRoom(room);
    planner.addMeeting(meeting1);
    planner.addMeeting(meeting2);
    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M2", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_TRUE(planner.getRooms()[0].isOccupied());
    EXPECT_FALSE(planner.getConflicts().empty());
}
TEST(MeetingPlannerTest, AutomaticallyProcessesMultipleMeetings) {
    MeetingPlanner planner;

    Room room1("A", "A101", 5);
    Room room2("B", "B202", 5);

    Meeting meeting1("Meeting1", "M1", "A101", "2026-03-20");
    Meeting meeting2("Meeting2", "M2", "B202", "2026-03-20");

    planner.addRoom(room1);
    planner.addRoom(room2);

    planner.addMeeting(meeting1);
    planner.addMeeting(meeting2);

    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M2", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 2);
    EXPECT_TRUE(planner.getRooms()[0].isOccupied());
    EXPECT_TRUE(planner.getRooms()[1].isOccupied());
}

TEST(MeetingPlannerTest, AutomaticallyContinuesAfterFailedMeeting) {
    MeetingPlanner planner;

    Room room("A", "A101", 5);
    Room room2("B", "B202", 5);

    Meeting meeting1("Meeting1", "M1", "A101", "2026-03-20");
    Meeting meeting2("Meeting2", "M2", "A101", "2026-03-20");
    Meeting meeting3("Meeting3", "M3", "B202", "2026-03-20");

    planner.addRoom(room);
    planner.addRoom(room2);

    planner.addMeeting(meeting1);
    planner.addMeeting(meeting2);
    planner.addMeeting(meeting3);

    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M2", "Bob");
    planner.addParticipation("M3", "Charlie");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 2);
    EXPECT_FALSE(planner.getConflicts().empty());
    EXPECT_TRUE(planner.getRooms()[1].isOccupied());
}
TEST(MeetingPlannerTest, AddCampusIncreasesCampusCount) {
    MeetingPlanner planner;
    Campus campus("Campus Drie Eiken", "CDE");

    planner.addCampus(campus);

    EXPECT_EQ(planner.getCampuses().size(), 1);
    EXPECT_EQ(planner.getCampuses()[0].getIdentifier(), "CDE");
}

TEST(MeetingPlannerTest, AddBuildingIncreasesBuildingCount) {
    MeetingPlanner planner;
    Campus campus("Campus Drie Eiken", "CDE");
    Building building("Gebouw R", "CDE_R", "CDE");

    planner.addCampus(campus);
    planner.addBuilding(building);

    EXPECT_EQ(planner.getBuildings().size(), 1);
    EXPECT_EQ(planner.getBuildings()[0].getIdentifier(), "CDE_R");
}