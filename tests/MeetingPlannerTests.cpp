//
// Created by firdi on 21/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/MeetingPlanner.h"
#include "week2_code/Room.h"
#include "week2_code/Meeting.h"
#include "week2_code/Campus.h"
#include "week2_code/Building.h"
#include "week2_code/Renovation.h"
#include "week2_code/CateringProvider.h"

TEST(MeetingPlannerTest, AddRoomIncreasesRoomCount) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Room room("M.G.023", "Room123", 10);

    planner.addRoom(room);

    EXPECT_EQ(planner.getRooms().size(), 1);
    EXPECT_EQ(planner.getRooms()[0].getIdentifier(), "Room123");
}

TEST(MeetingPlannerTest, AddMeetingIncreasesMeetingCount) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addMeeting(meeting);

    EXPECT_EQ(planner.getMeetings().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getIdentifier(), "Meeting123");
}

TEST(MeetingPlannerTest, AddParticipationToExistingMeetingWorks) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");
    planner.addMeeting(meeting);

    bool result = planner.addParticipation("Meeting123", "Alice");

    EXPECT_TRUE(result);
    EXPECT_EQ(planner.getMeetings()[0].getParticipants().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getParticipants()[0], "Alice");
}

TEST(MeetingPlannerTest, AddParticipationToUnknownMeetingFails) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    bool result = planner.addParticipation("BestaatNiet", "Alice");

    EXPECT_FALSE(result);
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsTrueForValidSystem) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Room room("M.G.023", "Room123", 10);
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("Meeting123", "Alice");

    EXPECT_TRUE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForDuplicateRoomIdentifiers) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Room room1("M.G.023", "Room123", 10);
    Room room2("M.G.024", "Room123", 15);

    planner.addRoom(room1);
    planner.addRoom(room2);

    EXPECT_FALSE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForUnknownRoomReference) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Meeting meeting("Weekly meeting", "Meeting123", "BestaatNiet", "2026-05-22");

    planner.addMeeting(meeting);

    EXPECT_FALSE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForDuplicateMeetingIdentifiers) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
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
    planner.setLoggingEnabled(false);
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
    planner.setLoggingEnabled(false);
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
    planner.setLoggingEnabled(false);
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
    planner.setLoggingEnabled(false);
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
    planner.setLoggingEnabled(false);
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
    planner.setLoggingEnabled(false);

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
    planner.setLoggingEnabled(false);

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
    planner.setLoggingEnabled(false);
    Campus campus("Campus Drie Eiken", "CDE");

    planner.addCampus(campus);

    EXPECT_EQ(planner.getCampuses().size(), 1);
    EXPECT_EQ(planner.getCampuses()[0].getIdentifier(), "CDE");
}

TEST(MeetingPlannerTest, AddBuildingIncreasesBuildingCount) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Campus campus("Campus Drie Eiken", "CDE");
    Building building("Gebouw R", "CDE_R", "CDE");

    planner.addCampus(campus);
    planner.addBuilding(building);

    EXPECT_EQ(planner.getBuildings().size(), 1);
    EXPECT_EQ(planner.getBuildings()[0].getIdentifier(), "CDE_R");
}

TEST(MeetingPlannerTest, AddRenovationIncreasesRenovationCount) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5);
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room);
    planner.addRenovation(renovation);

    EXPECT_EQ(planner.getRenovations().size(), 1);
    EXPECT_EQ(planner.getRenovations()[0].getRoomIdentifier(), "A101");
}

TEST(MeetingPlannerTest, AddCateringProviderIncreasesCateringProviderCount) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    CateringProvider provider("Campus_CDE", 20);

    planner.addCateringProvider(provider);

    EXPECT_EQ(planner.getCateringProviders().size(), 1);
    EXPECT_EQ(planner.getCateringProviders()[0].getCampusIdentifier(), "Campus_CDE");
    EXPECT_EQ(planner.getCateringProviders()[0].getCO2(), 20);
}

// USE CASE 3.3 - RENOVATIONS

TEST(MeetingPlannerTest, ProcessMeetingsCancelsMeetingWhenRoomIsUnderRenovation) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting("Team Meeting", "M1", "A101", "2026-04-15");
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addRenovation(renovation);

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 0);
    EXPECT_FALSE(planner.getRooms()[0].isOccupied());
    EXPECT_FALSE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, ProcessMeetingsCancelsMeetingOnRenovationStartDate) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting("Startdag Meeting", "M1", "A101", "2026-04-01");
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addRenovation(renovation);

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 0);
    EXPECT_FALSE(planner.getRooms()[0].isOccupied());
    EXPECT_FALSE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, ProcessMeetingsCancelsMeetingOnRenovationEndDate) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting("Einddag Meeting", "M1", "A101", "2026-06-01");
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addRenovation(renovation);

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 0);
    EXPECT_FALSE(planner.getRooms()[0].isOccupied());
    EXPECT_FALSE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, ProcessMeetingsAllowsMeetingOutsideRenovationPeriod) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting("Team Meeting", "M1", "A101", "2026-06-10");
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addRenovation(renovation);

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getIdentifier(), "M1");
    EXPECT_TRUE(planner.getRooms()[0].isOccupied());
    EXPECT_TRUE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, ProcessMeetingsAllowsMeetingWhenAnotherRoomIsUnderRenovation) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room1("Vergaderzaal A", "A101", 5);
    Room room2("Vergaderzaal B", "B202", 5);
    Meeting meeting("Team Meeting", "M1", "A101", "2026-04-15");
    Renovation renovation("B202", "2026-04-01", "2026-06-01");

    planner.addRoom(room1);
    planner.addRoom(room2);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addRenovation(renovation);

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getIdentifier(), "M1");
    EXPECT_TRUE(planner.getRooms()[0].isOccupied());
    EXPECT_FALSE(planner.getRooms()[1].isOccupied());
    EXPECT_TRUE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, AutomaticallyContinuesAfterMeetingBlockedByRenovation) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room1("Vergaderzaal A", "A101", 5);
    Room room2("Vergaderzaal B", "B202", 5);

    Meeting meeting1("Meeting In Renovatie", "M1", "A101", "2026-04-15");
    Meeting meeting2("Normale Meeting", "M2", "B202", "2026-04-15");

    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room1);
    planner.addRoom(room2);

    planner.addMeeting(meeting1);
    planner.addMeeting(meeting2);

    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M2", "Bob");

    planner.addRenovation(renovation);

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    EXPECT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getIdentifier(), "M2");
    EXPECT_FALSE(planner.getRooms()[0].isOccupied());
    EXPECT_TRUE(planner.getRooms()[1].isOccupied());
    EXPECT_FALSE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, TracksRoomOccupancyPercentageCorrectly) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 10);
    Meeting meeting("Team Meeting", "M1", "A101", "2026-04-15");

    planner.addRoom(room);
    planner.addMeeting(meeting);

    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M1", "Bob");
    planner.addParticipation("M1", "Charlie");
    planner.addParticipation("M1", "David");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);

    EXPECT_EQ(
            planner.getSuccessfulMeetings()[0].getOccupancyPercentage(),
            40
    );
}

TEST(MeetingPlannerTest, TracksFullRoomOccupancyAs100Percent) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Volle zaal", "FULL101", 2);
    Meeting meeting("Volle meeting", "M_FULL", "FULL101", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);

    planner.addParticipation("M_FULL", "Alice");
    planner.addParticipation("M_FULL", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getOccupancyPercentage(), 100);
}

TEST(MeetingPlannerTest, TracksLowRoomOccupancyCorrectly) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Grote zaal", "BIG101", 10);
    Meeting meeting("Kleine meeting", "M_SMALL", "BIG101", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);

    planner.addParticipation("M_SMALL", "Alice");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);

    EXPECT_EQ(
            planner.getSuccessfulMeetings()[0].getOccupancyPercentage(),
            10
    );
}

TEST(MeetingPlannerTest, MeetingBeforeRenovationPeriodIsAllowed) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting("Meeting Voor Renovatie", "M_BEFORE", "A101", "2026-03-20");
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M_BEFORE", "Alice");
    planner.addRenovation(renovation);

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getIdentifier(), "M_BEFORE");
    EXPECT_TRUE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, AddRenovationForUnknownRoomIsRejected) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Renovation renovation("UNKNOWN_ROOM", "2026-04-01", "2026-06-01");

    EXPECT_DEATH(
            planner.addRenovation(renovation),
            "Renovation moet verwijzen naar een bestaande room"
    );
}

// USE CASE 3.4 - ONLINE MEETINGS

TEST(MeetingPlannerTest, AddOnlineMeetingWithoutRoomIncreasesMeetingCount) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Meeting meeting("Online Meeting", "M_ONLINE", "", "2026-05-22");
    meeting.setOnline(true);

    planner.addMeeting(meeting);

    ASSERT_EQ(planner.getMeetings().size(), 1);
    EXPECT_EQ(planner.getMeetings()[0].getIdentifier(), "M_ONLINE");
    EXPECT_TRUE(planner.getMeetings()[0].isOnline());
    EXPECT_EQ(planner.getMeetings()[0].getRoomIdentifier(), "");
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsTrueForValidOnlineMeeting) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Meeting meeting("Online Meeting", "M_ONLINE", "", "2026-05-22");
    meeting.setOnline(true);

    planner.addMeeting(meeting);
    planner.addParticipation("M_ONLINE", "Alice");

    EXPECT_TRUE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, ProcessOnlineMeetingSucceedsWithoutOccupyingRoom) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5);
    Meeting meeting("Online Meeting", "M_ONLINE", "", "2026-05-22");
    meeting.setOnline(true);

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M_ONLINE", "Alice");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_TRUE(planner.getSuccessfulMeetings()[0].isOnline());
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getOccupancyPercentage(), 0);
    EXPECT_FALSE(planner.getRooms()[0].isOccupied());
    EXPECT_TRUE(planner.getConflicts().empty());
}

TEST(MeetingPlannerTest, AddPhysicalMeetingWithoutRoomIsRejected) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Meeting meeting("Fysieke Meeting", "M_PHYSICAL", "", "2026-05-22");

    EXPECT_DEATH(
            planner.addMeeting(meeting),
            "Fysieke meeting moet een room identifier hebben"
    );
}