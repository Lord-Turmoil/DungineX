/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : BaseInterface.h                           *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 19, 2025                          *
 *                                                                            *
 *                    Last Update : October 19, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Base interface definition.                                                 *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/Event/Event.h"
#include "DgeX/Core/Timer.h"
#include "DgeX/Utils/Types.h"

#include <string>

DGEX_BEGIN

namespace UI
{

class BaseInterface
{
public:
    BaseInterface() = default;
    virtual ~BaseInterface() = default;

    /**
     * @brief Get the name of the interface.
     *
     * Every interface should have a unique name to identify itself.
     *
     * @return Name of the interface.
     */
    DGEX_API const std::string& GetName() const;

public:
    virtual void Update(DeltaTime delta);
    virtual void OnEvent(const Ref<Event>& event);

private:
    std::string _name;

    Ref<FrameWidget> 
};

} // namespace UI

DGEX_END
