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
    Room room("M.G.023", "Room123", 10, "CDE", "CDE_R");

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
    Room room("M.G.023", "Room123", 10, "CDE", "CDE_R");
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("Meeting123", "Alice");

    EXPECT_TRUE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForDuplicateRoomIdentifiers) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);
    Room room1("M.G.023", "Room123", 10, "CDE", "CDE_R");
    Room room2("M.G.024", "Room123", 15, "CDE", "CDE_A");

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
    Room room("M.G.023", "Room123", 10, "CDE", "CDE_R");
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
    Room room("M.G.023", "Room123", 1, "CDE", "CDE_R");
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
    Room room("M.G.023", "Room123", 10, "CDE", "CDE_R");
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
    Room room("M.G.023", "Room123", 10, "CDE", "CDE_R");
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
    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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
    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

    Room room1("A", "A101", 5, "CDE", "CDE_A");
    Room room2("B", "B202", 5, "CDE", "CDE_B");

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

    Room room("A", "A101", 5, "CDE", "CDE_A");
    Room room2("B", "B202", 5, "CDE", "CDE_B");

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

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    planner.addRoom(room);
    planner.addRenovation(renovation);

    EXPECT_EQ(planner.getRenovations().size(), 1);
    EXPECT_EQ(planner.getRenovations()[0].getRoomIdentifier(), "A101");
}

TEST(MeetingPlannerTest, AddCateringProviderIncreasesCateringProviderCount) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    CateringProvider provider("Campus_CDE", 20.0f);

    planner.addCateringProvider(provider);

    EXPECT_EQ(planner.getCateringProviders().size(), 1);
    EXPECT_EQ(planner.getCateringProviders()[0].getCampusIdentifier(), "Campus_CDE");
    EXPECT_FLOAT_EQ(planner.getCateringProviders()[0].getCO2(), 20.0f);
}

// USE CASE 3.3 - RENOVATIONS

TEST(MeetingPlannerTest, ProcessMeetingsCancelsMeetingWhenRoomIsUnderRenovation) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

    Room room1("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
    Room room2("Vergaderzaal B", "B202", 5, "CDE", "CDE_B");
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

    Room room1("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
    Room room2("Vergaderzaal B", "B202", 5, "CDE", "CDE_B");

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

    Room room("Vergaderzaal A", "A101", 10, "CDE", "CDE_A");
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
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getOccupancyPercentage(), 40);
}

TEST(MeetingPlannerTest, TracksFullRoomOccupancyAs100Percent) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Volle zaal", "FULL101", 2, "CDE", "CDE_FULL");
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

    Room room("Grote zaal", "BIG101", 10, "CDE", "CDE_BIG");
    Meeting meeting("Kleine meeting", "M_SMALL", "BIG101", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);

    planner.addParticipation("M_SMALL", "Alice");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getOccupancyPercentage(), 10);
}

TEST(MeetingPlannerTest, MeetingBeforeRenovationPeriodIsAllowed) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
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

// USE CASE 3.5 - CO2 TRACKING

TEST(MeetingPlannerTest, ProcessOnlineMeetingTracksCO2Correctly) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Meeting meeting("Online Meeting", "M_ONLINE", "", "2026-05-22");
    meeting.setOnline(true);

    planner.addMeeting(meeting);
    planner.addParticipation("M_ONLINE", "Alice");
    planner.addParticipation("M_ONLINE", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_FLOAT_EQ(planner.getSuccessfulMeetings()[0].getCO2Emission(), 60.0f);
    EXPECT_FLOAT_EQ(planner.getTotalCO2Emission(), 60.0f);
}

TEST(MeetingPlannerTest, ProcessPhysicalMeetingTracksCO2CorrectlyWithoutCatering) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
    Meeting meeting("Fysieke Meeting", "M1", "A101", "2026-05-22");

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M1", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_FLOAT_EQ(planner.getSuccessfulMeetings()[0].getCO2Emission(), 240.0f);
    EXPECT_FLOAT_EQ(planner.getTotalCO2Emission(), 240.0f);
}

TEST(MeetingPlannerTest, ProcessPhysicalMeetingTracksCO2CorrectlyWithCatering) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
    Meeting meeting("Lunch Meeting", "M1", "A101", "2026-05-22");
    meeting.setCatering(true);

    CateringProvider provider("CDE", 20.0f);

    planner.addRoom(room);
    planner.addCateringProvider(provider);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");
    planner.addParticipation("M1", "Bob");

    ASSERT_TRUE(planner.checkConsistency());

    planner.processMeetings();

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    // 2 deelnemers => fysieke meeting: 2 * 120 = 240
    // catering: 2 * 20 = 40
    // totaal = 280
    EXPECT_FLOAT_EQ(planner.getSuccessfulMeetings()[0].getCO2Emission(), 280.0f);
    EXPECT_FLOAT_EQ(planner.getTotalCO2Emission(), 280.0f);
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseWhenCateringProviderMissingForCampus) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
    Meeting meeting("Lunch Meeting", "M1", "A101", "2026-05-22");
    meeting.setCatering(true);

    planner.addRoom(room);
    planner.addMeeting(meeting);
    planner.addParticipation("M1", "Alice");

    EXPECT_FALSE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, CheckConsistencyReturnsFalseForDuplicateCateringProvidersOnSameCampus) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    CateringProvider provider1("CDE", 20.0f);
    CateringProvider provider2("CDE", 25.0f);

    planner.addCateringProvider(provider1);
    planner.addCateringProvider(provider2);

    EXPECT_FALSE(planner.checkConsistency());
}

TEST(MeetingPlannerTest, FailedMeetingDoesNotIncreaseTotalCO2) {
    MeetingPlanner planner;
    planner.setLoggingEnabled(false);

    Room room1("Vergaderzaal A", "A101", 5, "CDE", "CDE_A");
    Room room2("Vergaderzaal B", "B202", 5, "CDE", "CDE_B");

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

    ASSERT_EQ(planner.getSuccessfulMeetings().size(), 1);
    EXPECT_EQ(planner.getSuccessfulMeetings()[0].getIdentifier(), "M2");

    // Enkel de geslaagde fysieke meeting telt mee:
    // 1 deelnemer * 120 = 120 CO2
    EXPECT_FLOAT_EQ(planner.getSuccessfulMeetings()[0].getCO2Emission(), 120.0f);
    EXPECT_FLOAT_EQ(planner.getTotalCO2Emission(), 120.0f);

    EXPECT_FALSE(planner.getConflicts().empty());
}