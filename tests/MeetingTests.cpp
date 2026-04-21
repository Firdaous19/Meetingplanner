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