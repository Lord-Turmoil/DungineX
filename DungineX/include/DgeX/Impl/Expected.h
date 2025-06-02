/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Expected.h                                *
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
 * Inspired by C++ 23 std::expected. Since we don't always want C++ 23, which *
 * is too advanced.                                                           *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <type_traits>
#include <utility>
#include <variant>

DGEX_BEGIN

template <typename T, typename E> class Expected
{
    static_assert(!std::is_same_v<T, void>, "T must not be void");
    static_assert(!std::is_same_v<E, void>, "E must not be void");
    static_assert(!std::is_same_v<T, E>, "T and E must not be the same type");
    static_assert(std::is_move_constructible_v<T>, "T must be move constructible");
    static_assert(std::is_move_constructible_v<E>, "E must be move constructible");

public:
    Expected(const Expected& other) : _value(other._value)
    {
    }

    Expected(Expected&& other) noexcept : _value(std::move(other._value))
    {
    }

    Expected& operator=(const Expected& other)
    {
        if (this != &other)
        {
            _value = other._value;
        }
        return *this;
    }

    Expected& operator=(Expected&& other) noexcept
    {
        if (this != &other)
        {
            _value = std::move(other._value);
        }
        return *this;
    }

    /**
     * @brief Constructs a succeeded Expected.
     *
     * Intended to be implicit.
     *
     * @tparam U Value type.
     * @param value Success value.
     */
    template <typename U = T, std::enable_if_t<std::is_constructible_v<T, U&&>, int> = 0>
    Expected(U&& value) : _value(std::in_place_type<T>, std::forward<U>(value))
    {
    }

    /**
     * @brief Constructs a failed Expected.
     *
     * Intended to be implicit.
     *
     * @tparam G Error type.
     * @param error Error value.
     */
    template <typename G = E, std::enable_if_t<std::is_constructible_v<E, G&&>, int> = 0>
    Expected(G&& error, std::enable_if_t<!std::is_same_v<std::decay_t<G>, Expected>, int>* = nullptr)
        : _value(std::in_place_type<E>, std::forward<G>(error))
    {
    }

    /**
     * @brief Check if it is expected.
     *
     * If you try to get value on a failed Expected, then the
     * underlying std::variant will throw an exception.
     *
     * @return Whether it is expected or not.
     */
    bool IsExpected() const
    {
        return std::holds_alternative<T>(_value);
    }

    const T& Value() const&
    {
        return std::get<T>(_value);
    }

    const T&& Value() const&&
    {
        return std::move(std::get<T>(_value));
    }

    const E& Error() const&
    {
        return std::get<E>(_value);
    }

    const E&& Error() const&&
    {
        return std::move(std::get<E>(_value));
    }

    /**
     * @brief implicit bool conversion.
     */
    operator bool() const
    {
        return IsExpected();
    }

private:
    std::variant<T, E> _value;
};

// Just two markers for clarity.
#define Success(x) (x)
#define Failure(x) (x)

DGEX_END
