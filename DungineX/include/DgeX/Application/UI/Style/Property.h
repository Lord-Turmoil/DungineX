/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Property.h                                *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 4, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Base definition for properties.                                            *
 ******************************************************************************/

#pragma once

#include <utility>

#include "DgeX/Core/Timer.h"
#include "DgeX/Renderer/Color.h"
#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Easing.h"

DGEX_BEGIN

namespace UI
{

template <typename T>
struct PropertyTypeTrait
{
    using value_type = T;
    using reference_type = T;
};

template <>
struct PropertyTypeTrait<std::string>
{
    using value_type = std::string;
    using reference_type = const std::string&;
};

template <typename T>
class Property
{
    using value_type = typename PropertyTypeTrait<T>::value_type;
    using reference_type = typename PropertyTypeTrait<T>::reference_type;

public:
    Property(value_type value) : _value(std::move(value))
    {
    }

    reference_type Value() const
    {
        return _value;
    }

    virtual ~Property() = default;

    /**
     * @brief For dynamic properties affected by transition.
     * @param delta Elapsed time since last update.
     */
    virtual void Update(DeltaTime delta)
    {
    }

protected:
    value_type _value;
};

template <typename T>
class DynamicProperty : public Property<T>
{
    using value_type = typename PropertyTypeTrait<T>::value_type;
    using reference_type = typename PropertyTypeTrait<T>::reference_type;

public:
    DynamicProperty(reference_type initial, reference_type target, timestamp_t duration,
                    Utils::Easing::EasingFn<timestamp_t> fn)
        : Property<T>(initial), _initial(initial), _target(target), _fn(fn), _duration(duration), _elapsed(0)
    {
        DGEX_ASSERT(_fn, "Easing function cannot be null");
    }

    ~DynamicProperty() override = default;

    void Update(DeltaTime delta) override
    {
        _elapsed += delta.Seconds();
        if (_elapsed >= _duration)
        {
            this->_value = _target;
            _elapsed = _duration;
        }
        else
        {
            this->_value = Utils::Easing::Interpolate<value_type, timestamp_t>(
                _initial, _target, Utils::Easing::RangeToStep<timestamp_t>(0, _duration, _elapsed), _fn);
        }
    }

protected:
    value_type _initial;
    value_type _target;
    Utils::Easing::EasingFn<timestamp_t> _fn;
    timestamp_t _duration;
    timestamp_t _elapsed;
};


} // namespace UI

DGEX_END
