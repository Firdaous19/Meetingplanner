#include <gtest/gtest.h>
#include "week2_code/Room.h"

TEST(RoomTest, AddPersonToRoom) {
    Room kamer("TestKamer", "TK1", 2);

    kamer.addPerson("Gebruiker 1");

    EXPECT_EQ(kamer.getName(), "TestKamer");
    EXPECT_EQ(kamer.getIdentifier(), "TK1");
    EXPECT_EQ(kamer.getCapacity(), 2);
}
