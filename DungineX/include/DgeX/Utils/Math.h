/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Math.h                                    *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 2, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Utility math functions.                                                    *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <cmath>

DGEX_BEGIN

namespace Math
{

template <typename T> constexpr T PI = T(3.14159265358979323846);
template <typename T> constexpr T TWO_PI = T(2.0) * PI<T>;
template <typename T> constexpr T HALF_PI = T(0.5) * PI<T>;
template <typename T> constexpr T QUARTER_PI = T(0.25) * PI<T>;

template <typename T> constexpr T ToRadians(T degrees)
{
    return degrees * PI<T> / T(180.0f);
}

template <typename T> constexpr T ToDegrees(T radians)
{
    return radians * T(180.0f) / PI<T>;
}

template <typename T> constexpr T Min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T> constexpr T Max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T> constexpr T Clamp(T value, T min, T max)
{
    return Min(Max(value, min), max);
}

template <typename T> constexpr T Lerp(T a, T b, T t)
{
    return a + (b - a) * t;
}

template <typename T> constexpr T Cos(T x)
{
    return std::cos(x);
}

template <typename T> constexpr T Sin(T x)
{
    return std::sin(x);
}

template <typename T> constexpr T Tan(T x)
{
    return std::tan(x);
}

template <typename T> constexpr T Pow(T x, T y)
{
    return std::pow(x, y);
}

template <typename T> constexpr T Pow2(T x)
{
    return x * x;
}

template <typename T> constexpr T Pow3(T x)
{
    return x * x * x;
}

template <typename T> constexpr T Pow4(T x)
{
    return x * x * x * x; // Guess can be optimized as (x * x) ^ 2
}

template <typename T> constexpr T Pow5(T x)
{
    return x * x * x * x * x; // Guess can be optimized as (x * x) ^ 2 * x
}

template <typename T> constexpr T Abs(T x)
{
    return std::abs(x);
}

template <typename T> constexpr T Sqrt(T x)
{
    return std::sqrt(x);
}

template <typename T> constexpr T Exp(T x)
{
    return std::exp(x);
}

} // namespace Math

DGEX_END
