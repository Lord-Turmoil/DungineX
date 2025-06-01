/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Types.h                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 1, 2025                              *
 *                                                                            *
 *                    Last Update : June 1, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Common type declarations.                                                  *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <memory>

DGEX_BEGIN

/**
 * We use Scope for std::unique_ptr.
 */
template <typename T> using Scope = std::unique_ptr<T>;

template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * We use Ref for std::shared_ptr.
 */
template <typename T> using Ref = std::shared_ptr<T>;

template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/**
 * Just an alias for raw pointers.
 */
template <typename T> using Ptr = T*;

DGEX_END
