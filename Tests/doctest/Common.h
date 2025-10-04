#pragma once

#include "doctest.h"

#include <filesystem>

template <typename... Args> std::filesystem::path GetResourceImpl(Args... filenames);

template <typename T, typename... Args> std::filesystem::path GetResourceImpl(const T& filename, Args... filenames)
{
    return filename / GetResourceImpl(filenames...);
}

template <typename T> std::filesystem::path GetResourceImpl(const T& filename)
{
    return filename;
}

template <typename... Args> std::filesystem::path GetResource(Args... filenames)
{
    return std::filesystem::current_path() / "Resources" / GetResourceImpl(filenames...);
}

#include <cstring>

#define CHECK_STR_EQ(X, Y)   CHECK_EQ(strcmp(X, Y), 0)
#define REQUIRE_STR_EQ(X, Y) REQUIRE_EQ(strcmp(X, Y), 0)
