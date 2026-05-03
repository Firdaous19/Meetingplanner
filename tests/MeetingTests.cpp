//
// Created by firdi on 21/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/Meeting.h"

TEST(MeetingTest, ConstructorStoresValuesCorrectly) {
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    EXPECT_EQ(meeting.getLabel(), "Weekly meeting");
    EXPECT_EQ(meeting.getIdentifier(), "Meeting123");
    EXPECT_EQ(meeting.getRoomIdentifier(), "Room123");
    EXPECT_EQ(meeting.getDate(), "2026-05-22");
    EXPECT_EQ(meeting.getParticipants().size(), 0);
    EXPECT_FLOAT_EQ(meeting.getCO2Emission(), 0.0f);
    EXPECT_EQ(meeting.getOccupancyPercentage(), 0);
}

TEST(MeetingTest, AddParticipantIncreasesParticipantCount) {
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    meeting.addParticipant("Alice");

    EXPECT_EQ(meeting.getParticipants().size(), 1);
    EXPECT_EQ(meeting.getParticipants()[0], "Alice");
}

TEST(MeetingTest, ValidDateFormatIsAccepted) {
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    EXPECT_EQ(meeting.getDate(), "2026-05-22");
}

TEST(MeetingTest, InvalidDateFormatIsRejected) {
    EXPECT_DEATH(
            Meeting("Weekly meeting", "Meeting123", "Room123", "22-05-2026"),
            "Meeting date moet formaat YYYY-MM-DD hebben"
    );
}

TEST(MeetingTest, InvalidMonthIsRejected) {
    EXPECT_DEATH(
            Meeting("Weekly meeting", "Meeting123", "Room123", "2026-15-22"),
            "Meeting date moet formaat YYYY-MM-DD hebben"
    );
}

TEST(MeetingTest, InvalidDayIsRejected) {
    EXPECT_DEATH(
            Meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-99"),
            "Meeting date moet formaat YYYY-MM-DD hebben"
    );
}

TEST(MeetingTest, NegativeCO2EmissionIsRejected) {
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    EXPECT_DEATH(
            meeting.setCO2Emission(-1.0f),
            "CO2 emission mag niet negatief zijn"
    );
}

TEST(MeetingTest, InvalidOccupancyPercentageIsRejected) {
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    EXPECT_DEATH(
            meeting.setOccupancyPercentage(150),
            "Occupancy percentage moet tussen 0 en 100 liggen"
    );
}

TEST(MeetingTest, ValidCO2AndOccupancyValuesAreStoredCorrectly) {
    Meeting meeting("Weekly meeting", "Meeting123", "Room123", "2026-05-22");

    meeting.setOnline(false);
    meeting.setCatering(true);
    meeting.setCO2Emission(250.0f);
    meeting.setOccupancyPercentage(80);
    meeting.setExternalsAllowed(true);

    EXPECT_FLOAT_EQ(meeting.getCO2Emission(), 250.0f);
    EXPECT_EQ(meeting.getOccupancyPercentage(), 80);
    EXPECT_TRUE(meeting.hasCatering());
    EXPECT_FALSE(meeting.isOnline());
    EXPECT_TRUE(meeting.areExternalsAllowed());
}

TEST(MeetingTest, OnlineMeetingCannotHaveCatering) {
    Meeting meeting("Online meeting", "Meeting999", "", "2026-05-22");

    meeting.setOnline(true);

    EXPECT_DEATH(
            meeting.setCatering(true),
            "Online meeting mag geen catering hebben"
    );
}