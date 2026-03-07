#include <gtest/gtest.h>
#include "Room.h"

//Deze test dient om te controleren of de Room correct is aangemaakt
TEST(RoomTest, CreateRoomValid) {
    Room meetingRoom("Vergaderzaal A", 10);

    EXPECT_EQ(meetingRoom.getName(), "Vergaderzaal A");
    EXPECT_EQ(meetingRoom.getCapacity(), 10);
}

//De test controleert of de capaciteit positief is
TEST(RoomTest, CapacityPositive) {
    Room B("B202", 10);

    EXPECT_GT(B.getCapacity(), 0);
}