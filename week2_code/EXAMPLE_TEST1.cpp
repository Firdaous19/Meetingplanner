#include <gtest/gtest.h>
#include "Room.h" // Jouw eigen klasse
#include "../DesignByContract.h" // Voor de checks

TEST(RoomTest, CreateRoomValid) {
    Room room("Vergaderzaal A", 10);

    EXPECT_EQ(room.getName(), "Vergaderzaal A");
    EXPECT_EQ(room.getCapacity(), 10);
}

TEST(RoomTest, CapacityPositive) {
    Room B("B202", 10);
    EXPECT_GT(B.getCapacity(), 0);
}