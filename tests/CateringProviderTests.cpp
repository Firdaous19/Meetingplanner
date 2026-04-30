//
// Created by firdi on 28/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/CateringProvider.h"

TEST(CateringProviderTest, ConstructorStoresValuesCorrectly) {
CateringProvider provider("Campus_CDE", 20);

EXPECT_EQ(provider.getCampusIdentifier(), "Campus_CDE");
EXPECT_EQ(provider.getCO2(), 20);
}

TEST(CateringProviderTest, EmptyCampusIdentifierIsRejected) {
EXPECT_DEATH(
        CateringProvider("", 20),
"Campus identifier mag niet leeg zijn"
);
}

TEST(CateringProviderTest, NegativeCO2IsRejected) {
EXPECT_DEATH(
        CateringProvider("Campus_CDE", -5),
"CO2 mag niet negatief zijn"
);
}
