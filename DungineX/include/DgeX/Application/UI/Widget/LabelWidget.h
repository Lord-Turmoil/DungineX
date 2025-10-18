/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : LabelWidget.h                             *
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
 * Label widget has text.                                                     *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/UI/Widget/Widget.h"

#include <string>

DGEX_BEGIN

namespace UI
{

class LabelWidget : public Widget
{
public:
    LabelWidget(const WidgetContext& context, const Ext::XmlElement& element);

    /**
     * @brief Set the text.
     *
     * @param text The text of the label widget.
     */
    void SetText(const std::string& text);

    /**
     * @brief Get the text.
     *
     * @return The text of the label widget.
     */
    const std::string& GetText() const;

private:
    void _ApplyWidth(const Widget& parent) override;
    void _ApplyHeight(const Widget& parent) override;

private:
    std::string _text;
};

} // namespace UI

DGEX_END
