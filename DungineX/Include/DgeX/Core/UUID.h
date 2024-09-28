#pragma once

#include "DgeX/Common/Common.h"

DGEX_BEGIN

// ReSharper disable once CppInconsistentNaming
class UUID
{
public:
    UUID();
    UUID(uint64_t uuid);
    ~UUID() = default;

    operator uint64_t() const
    {
        return _uuid;
    }

    bool operator==(const UUID& other) const
    {
        return _uuid == other._uuid;
    }

    std::string ToString() const;

private:
    uint64_t _uuid;
};

DGEX_END

namespace std
{
template <typename T> struct hash;

template <> struct hash<DgeX::UUID>
{
    std::size_t operator()(const DgeX::UUID& uuid) const noexcept
    {
        return uuid;
    }
};

template <> struct equal_to<DgeX::UUID>
{
    bool operator()(const DgeX::UUID& lhs, const DgeX::UUID& rhs) const noexcept
    {
        return lhs == rhs;
    }
};

} // namespace std

/**
 * @brief spdlog custom logging
 */
template <typename OStream> OStream& operator<<(OStream& os, const DgeX::UUID& obj)
{
    fmt::format_to(std::ostream_iterator<char>(os), "{}", obj.ToString());
    return os;
}
