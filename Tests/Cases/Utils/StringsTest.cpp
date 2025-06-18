#include "doctest/doctest.h"

#include <DgeX/DgeX.h>

TEST_CASE("Strings Test")
{
    CHECK(DgeX::Strings::StartsWith("DungineX", "Dun"));
    CHECK_FALSE(DgeX::Strings::StartsWith("DungineX", "dun"));
    CHECK_FALSE(DgeX::Strings::StartsWith("DungineX", "DungineX "));

    CHECK(DgeX::Strings::EndsWith("DungineX", "ineX"));
    CHECK_FALSE(DgeX::Strings::EndsWith("DungineX", "ine"));
    CHECK_FALSE(DgeX::Strings::EndsWith("DungineX", " DungineX"));
}
