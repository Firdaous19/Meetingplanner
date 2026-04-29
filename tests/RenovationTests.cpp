#include "gtest/gtest.h"
#include "week2_code/Renovation.h"

TEST(RenovationTest, ConstructorStoresValuesCorrectly) {
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    EXPECT_EQ(renovation.getRoomIdentifier(), "A101");
    EXPECT_EQ(renovation.getStartDate(), "2026-04-01");
    EXPECT_EQ(renovation.getEndDate(), "2026-06-01");
}

TEST(RenovationTest, EmptyRoomIdentifierIsRejected) {
    EXPECT_DEATH(
            Renovation("", "2026-04-01", "2026-06-01"),
            "Renovation room identifier mag niet leeg zijn"
    );
}

TEST(RenovationTest, EmptyStartDateIsRejected) {
    EXPECT_DEATH(
            Renovation("A101", "", "2026-06-01"),
            "Renovation start date mag niet leeg zijn"
    );
}

TEST(RenovationTest, EmptyEndDateIsRejected) {
    EXPECT_DEATH(
            Renovation("A101", "2026-04-01", ""),
            "Renovation end date mag niet leeg zijn"
    );
}

TEST(RenovationTest, IsActiveOnDateReturnsTrueWithinAndOnBoundsOfRenovationPeriod) {
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    EXPECT_TRUE(renovation.isActiveOnDate("2026-04-01"));
    EXPECT_TRUE(renovation.isActiveOnDate("2026-05-15"));
    EXPECT_TRUE(renovation.isActiveOnDate("2026-06-01"));
}

TEST(RenovationTest, IsActiveOnDateReturnsFalseOutsideRenovationPeriod) {
    Renovation renovation("A101", "2026-04-01", "2026-06-01");

    EXPECT_FALSE(renovation.isActiveOnDate("2026-03-31"));
    EXPECT_FALSE(renovation.isActiveOnDate("2026-06-02"));
}