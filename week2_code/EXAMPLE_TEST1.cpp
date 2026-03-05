//
// Created by ali_s on 5/03/2026.
//
#include <gtest/gtest.h>
#include "Room.h"

// Eerste test voor Room
TEST(RoomTest, CreateRoomValid) {

    Room room("Vergaderzaal A", 10);

    EXPECT_EQ(room.getName(), "Vergaderzaal A");
    EXPECT_EQ(room.getCapacity(), 10);

}