#include "doctest/doctest.h"

#include <DgeX/DgeX.h>

TEST_CASE("Strings Test")
{
    using namespace DgeX::Strings;

    SUBCASE("std::string")
    {
        CHECK(StartsWith(std::string("DungineX"), "Dun"));
        CHECK_FALSE(StartsWith(std::string("DungineX"), "dun"));
        CHECK_FALSE(StartsWith(std::string("DungineX"), "DungineX "));

        CHECK(EndsWith(std::string("DungineX"), "ineX"));
        CHECK_FALSE(EndsWith(std::string("DungineX"), "ine"));
        CHECK_FALSE(EndsWith(std::string("DungineX"), " DungineX"));
    }

    SUBCASE("const char*")
    {
        CHECK(StartsWith("DungineX", "Dun"));
        CHECK_FALSE(StartsWith("DungineX", "dun"));
        CHECK_FALSE(StartsWith("DungineX", "DungineX "));

        CHECK(EndsWith("DungineX", "ineX"));
        CHECK_FALSE(EndsWith("DungineX", "ine"));
        CHECK_FALSE(EndsWith("DungineX", " DungineX"));
    }
}
