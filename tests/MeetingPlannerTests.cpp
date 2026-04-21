//
// Created by firdi on 21/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/MeetingPlanner.h"
#include "week2_code/Room.h"
#include "week2_code/Meeting.h"

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