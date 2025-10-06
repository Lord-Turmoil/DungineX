#pragma once

#include "doctest.h"

#include <cstring>
#include <filesystem>

template <typename T>
std::filesystem::path GetResourceImpl(const T& filename)
{
    return filename;
}

template <typename T, typename... Args>
std::filesystem::path GetResourceImpl(const T& filename, Args... filenames)
{
    return filename / GetResourceImpl(filenames...);
}

template <typename... Args>
std::filesystem::path GetResource(Args... filenames)
{
    if (sizeof...(filenames) == 0)
    {
        return std::filesystem::current_path() / "Resources";
    }
    return std::filesystem::current_path() / "Resources" / GetResourceImpl(std::string(filenames)...);
}

#define CHECK_STR_EQ(X, Y)   CHECK_EQ(strcmp(X, Y), 0)
#define REQUIRE_STR_EQ(X, Y) REQUIRE_EQ(strcmp(X, Y), 0)
