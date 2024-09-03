#pragma once

#include "DgeX/Core/Math.h"

DGEX_BEGIN

/**
 * @brief Collection of easing functions.
 * @ref https://nicmulvaney.com/easing
 */
namespace Utils::Easing
{

// The easing function can be defined as follows, it starts at 0 and ends at 1.
//        / 0    , t = 0
// f(t) = | e(t) , 0 < t < 1
//        \ 1    , t = 1
template <typename T> using EasingFn = T (*)(T);

/**
 * @brief Convert range of time to step in [0, 1]
 * @param begin the time to begin interpolate, time before will be clamped to 0
 * @param end the time to end interpolate, time after will be clamped to 1
 * @param t timestamp in time range
 * @warning begin should be less than end, and not be the same
 * @return interpolate step in [0, 1]
 */
template <typename T> constexpr T RangeToStep(T begin, T end, T t)
{
    return (Math::Clamp(t, begin, end) - begin) / (end - begin);
}

/**
 * @brief Interpolate between initial and target value with easing function
 * @param initial initial value for the interpolation
 * @param target target value for the interpolation
 * @param step step in [0, 1] to interpolate between begin and end
 * @param fn easing function to apply
 * @note step should be in [0, 1], it's OK to be out of range, but the result
 *      may not be as expected
 * @note call RangeToStep to convert time range to step
 * @return interpolated value
 */
template <typename T, typename S> constexpr T Interpolate(T initial, T target, S step, EasingFn<S> fn)
{
    return static_cast<T>(initial + (target - initial) * fn(step));
}

template <typename T> constexpr T Linear(T t)
{
    return t;
}

template <typename T> constexpr T EaseInSine(T x)
{
    return T(1) - Math::Cos((x * Math::PI<T>) / T(2));
}

template <typename T> constexpr T EaseOutSine(T x)
{
    return -(Math::Cos(Math::PI<T> * x) - T(1)) / T(2);
}

template <typename T> constexpr T EaseInOutSine(T x)
{
    return -(Math::Cos(Math::PI<T> * x) - T(1)) / T(2);
}

template <typename T> constexpr T EaseInQuad(T x)
{
    return Math::Pow2(x);
}

template <typename T> constexpr T EaseOutQuad(T x)
{
    return T(1) - Math::Pow2(T(1) - x);
}

template <typename T> constexpr T EaseInOutQuad(T x)
{
    return x < T(0.5) ? (T(2) * Math::Pow2(x)) : (T(1) - Math::Pow2(T(-2) * x + T(2)) / T(2));
}

template <typename T> constexpr T EaseInCubic(T x)
{
    return Math::Pow3(x);
}

template <typename T> constexpr T EaseOutCubic(T x)
{
    return T(1) - Math::Pow3(T(1) - x);
}

template <typename T> constexpr T EaseInOutCubic(T x)
{
    return x < T(0.5) ? (T(4) * Math::Pow3(x)) : (T(1) - Math::Pow3(T(-2) * x + T(2)) / T(2));
}

template <typename T> constexpr T EaseInQuart(T x)
{
    return Math::Pow4(x);
}

template <typename T> constexpr T EaseOutQuart(T x)
{
    return T(1) - Math::Pow4(T(1) - x);
}

template <typename T> constexpr T EaseInOutQuart(T x)
{
    return x < T(0.5) ? (T(8) * Math::Pow4(x)) : (T(1) - Math::Pow4(T(-2) * x + T(2)) / T(2));
}

template <typename T> constexpr T EaseInQuint(T x)
{
    return Math::Pow5(x);
}

template <typename T> constexpr T EaseOutQuint(T x)
{
    return T(1) - Math::Pow5(T(1) - x);
}

template <typename T> constexpr T EaseInOutQuint(T x)
{
    return x < T(0.5) ? (T(16) * Math::Pow5(x)) : (T(1) - Math::Pow5(T(-2) * x + T(2)) / T(2));
}

template <typename T> constexpr T EaseInExpo(T x)
{
    return x == T(0) ? T(0) : Math::Pow(T(2), T(10) * x - T(10));
}

template <typename T> constexpr T EaseOutExpo(T x)
{
    return x == T(1) ? T(1) : T(1) - Math::Pow(T(2), T(-10) * x);
}

template <typename T> constexpr T EaseInOutExpo(T x)
{
    return x == T(0)    ? T(0)
           : x == T(1)  ? T(1)
           : x < T(0.5) ? Math::Pow(T(2), T(20) * x - T(10)) / T(2)
                        : (T(2) - Math::Pow(T(2), T(-20) * x + T(10))) / T(2);
}

template <typename T> constexpr T EaseInCirc(T x)
{
    return T(1) - Math::Sqrt(T(1) - Math::Pow2(x));
}

template <typename T> constexpr T EaseOutCirc(T x)
{
    return Math::Sqrt(T(1) - Math::Pow2(x - T(1)));
}

template <typename T> constexpr T EaseInOutCirc(T x)
{
    return x < T(0.5) ? (T(1) - Math::Sqrt(T(1) - Math::Pow2(T(2) * x))) / T(2)
                      : (Math::Sqrt(T(1) - Math::Pow2(T(-2) * x + T(2))) + T(1)) / T(2);
}

template <typename T> constexpr T EaseInBack(T x)
{
    return EaseInBack(x, 1.70158);
}

/**
 * @brief with full parameter
 * @param c > 0, c == 0 will fall back to cubic
 * @return
 */
template <typename T> constexpr T EaseInBack(T x, T c)
{
    return (c + T(1)) * Math::Pow3(x) - c * Math::Pow2(x);
}

template <typename T> constexpr T EaseOutBack(T x)
{
    return EaseOutBack(x, 1.70158);
}

template <typename T> constexpr T EaseOutBack(T x, T c)
{
    return T(1) + (c + T(1)) * Math::Pow3(x - T(1)) + c * Math::Pow2(x - T(1));
}

template <typename T> constexpr T EaseInOutBack(T x)
{
    return EaseInOutBack(x, 1.70158);
}

template <typename T> constexpr T EaseInOutBack(T x, T c)
{
    const T s = c * T(1.525);
    return x < T(0.5) ? (Math::Pow2(T(2) * x) * ((s + T(1)) * T(2) * x - s)) / T(2)
                      : (Math::Pow2(T(2) * x - T(2)) * ((s + T(1)) * (T(2) * x - T(2)) + s) + T(2)) / T(2);
}

template <typename T> constexpr T EaseInElastic(T x)
{
    const T c = T(2) * Math::PI<T> / T(3);
    return x == 0 ? T(0) : x == 1 ? T(1) : -Math::Pow(T(2), T(10) * x - T(10)) * Math::Sin((x * T(10) - T(10.75)) * c);
}

template <typename T> constexpr T EaseOutElastic(T x)
{
    const T c = T(2) * Math::PI<T> / T(3);
    return x == 0 ? T(0) : x == 1 ? T(1) : Math::Pow(T(2), T(-10) * x) * Math::Sin((x * T(10) - T(0.75)) * c) + T(1);
}

template <typename T> constexpr T EaseInOutElastic(T x)
{
    const T c = T(2) * Math::PI<T> / T(4.5);
    return x == 0       ? T(0)
           : x == 1     ? T(1)
           : x < T(0.5) ? -Math::Pow(T(2), T(20) * x - T(10)) * Math::Sin((x * T(20) - T(11.125)) * c) / T(2)
                        : Math::Pow(T(2), T(-20) * x + T(10)) * Math::Sin((x * T(20) - T(11.125)) * c) / T(2) + T(1);
}

template <typename T> constexpr T EaseInBounce(T x)
{
    return T(1) - EaseOutBounce(T(1) - x);
}

template <typename T> constexpr T EaseOutBounce(T x)
{
    const T c = 7.5625;
    const T s = 2.75;
    if (x < 1 / s)
    {
        return c * x * x;
    }
    if (x < 2 / s)
    {
        return c * (x -= 1.5 / s) * x + 0.75;
    }
    if (x < 2.5 / s)
    {
        return c * (x -= 2.25 / s) * x + 0.9375;
    }
    return c * (x -= 2.625 / s) * x + 0.984375;
}

template <typename T> constexpr T EaseInOutBounce(T x)
{
    return x < T(0.5) ? (T(1) - EaseOutBounce(T(1) - 2 * x)) / T(2) : (T(1) + EaseOutBounce(2 * x - 1)) / T(2);
}

} // namespace Utils::Easing

DGEX_END
