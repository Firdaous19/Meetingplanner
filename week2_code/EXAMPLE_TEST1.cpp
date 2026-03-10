#include <gtest/gtest.h>

TEST(RoomTest, CreateRoomValid) {

}

TEST(RoomTest, CapacityPositive) {
    Room B("B202", 10);
    EXPECT_GT(B.getCapacity(), 0);
}