/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetContext.h                           *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 11, 2025                          *
 *                                                                            *
 *                    Last Update : October 11, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Context during the loading of the widget. It includes styles, and maybe    *
 * also some metadata in the future. It does not hold resources, which are    *
 * handled by resource manager.                                               *
 ******************************************************************************/

#pragma once

#include "DgeX/Utils/Types.h"

#include <string>
#include <unordered_map>

DGEX_BEGIN

namespace UI
{

class Style;

/**
 * @brief The context holds all styles and metadata during widget loading.
 */
class WidgetContext
{
public:
    WidgetContext() = default;

    /**
     * @brief Add a style to the context.
     *
     * @param style The style to add.
     */
    void AddStyle(const Ref<Style>& style);

    /**
     * @brief Get a style with the given name.
     *
     * @param name The name of the style.
     * @return The style of the name, or nullptr if the style does not exits.
     */
    Ref<Style> GetStyle(const std::string& name) const;

private:
    std::unordered_map<std::string, Ref<Style>> _styles;
};

} // namespace UI

DGEX_END
