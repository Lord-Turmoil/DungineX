#pragma once

#include <memory>

#include "DgeX/Common/Macros.h"

DGEX_BEGIN

template <typename T> using Scope = std::unique_ptr<T>;

template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;

template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

DGEX_END

#define DGEX_EXPAND_MACRO(x)    x
#define DGEX_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << (x))

#define DGEX_BIND_EVENT_FN(fn)                                                                                         \
    [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// Common headers
#include "DgeX/Common/Assert.h"
#include "DgeX/Common/Log.h"
