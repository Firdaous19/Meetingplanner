//
// Created by firdi on 28/04/2026.
//
#include "gtest/gtest.h"
#include "week2_code/CateringProvider.h"

TEST(CateringProviderTest, ConstructorStoresValuesCorrectly) {
        CateringProvider provider("Campus_CDE", 20.0f);

        EXPECT_EQ(provider.getCampusIdentifier(), "Campus_CDE");
        EXPECT_FLOAT_EQ(provider.getCO2(), 20.0f);
}

TEST(CateringProviderTest, EmptyCampusIdentifierIsRejected) {
        EXPECT_DEATH(
                CateringProvider("", 20.0f),
                "Campus identifier mag niet leeg zijn"
        );
}

TEST(CateringProviderTest, ZeroCO2IsRejected) {
        EXPECT_DEATH(
                CateringProvider("Campus_CDE", 0.0f),
                "CO2 moet groter zijn dan 0"
        );
}

TEST(CateringProviderTest, NegativeCO2IsRejected) {
        EXPECT_DEATH(
                CateringProvider("Campus_CDE", -5.0f),
                "CO2 moet groter zijn dan 0"
        );
}
