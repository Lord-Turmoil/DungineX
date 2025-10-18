/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : FrameWidget.h                             *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 18, 2025                          *
 *                                                                            *
 *                    Last Update : October 18, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Top-level widget.                                                          *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/Event/WindowEvents.h"
#include "DgeX/Application/UI/Widget/Widget.h"

DGEX_BEGIN

namespace UI
{

/**
 * @brief The top-level widget.
 *
 * Frame is the top-level widget. It should not have a parent, and only have one state.
 */
class FrameWidget : public Widget
{
public:
    FrameWidget(std::string id);
    FrameWidget(const WidgetContext& context, const Ext::XmlElement& element);

public:
    /**
     * @brief Frame should not have parent, and only have one state.
     */
    void OnEvent(const Ref<Event>& event) override;

    void ApplyStyles() override;

private:
    bool _OnWindowResized(const WindowResizedEvent& event);
};

} // namespace UI

DGEX_END
