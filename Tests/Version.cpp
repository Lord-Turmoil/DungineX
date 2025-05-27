#include "doctest/doctest.h"

#include <DgeX/DgeX.h>

TEST_CASE("Get Version")
{
    int major, minor, patch;

    DgeX::GetDgeXVersion(&major, &minor, &patch);
    CHECK_EQ(major, DGEX_VERSION_MAJOR);
    CHECK_EQ(minor, DGEX_VERSION_MINOR);
    CHECK_EQ(patch, DGEX_VERSION_PATCH);

    std::string version = DgeX::GetDgeXVersion();
    CHECK_EQ(version, std::to_string(DGEX_VERSION_MAJOR) + "." + std::to_string(DGEX_VERSION_MINOR) + "." +
                          std::to_string(DGEX_VERSION_PATCH));
}
