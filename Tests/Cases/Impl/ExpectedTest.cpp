#include "doctest/doctest.h"

#include <DgeX/DgeX.h>

struct Good
{
    int Value;
};

TEST_CASE("Expected Test")
{
    DgeX::Expected<Good, int> expected = Success(Good{ 1 });
    CHECK(expected.IsExpected());
    CHECK(expected); // implicit convertion to bool
    CHECK_EQ(expected.Value().Value, 1);

    DgeX::Expected<Good, int> expected = Failure(2);
    CHECK(!expected.IsExpected());
    CHECK(!expected);
    CHECK_EQ(expected.Error(), 2);
}
