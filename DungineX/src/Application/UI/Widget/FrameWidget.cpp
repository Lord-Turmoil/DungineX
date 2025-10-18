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

#include "DgeX/Application/UI/Widget/FrameWidget.h"

#include "DgeX/Renderer/Font.h"
#include "DgeX/Utils/Strings.h"

DGEX_BEGIN

namespace UI
{

FrameWidget::FrameWidget(std::string id) : Widget("Frame", std::move(id))
{
}

FrameWidget::FrameWidget(const WidgetContext& context, const Ext::XmlElement& element) : Widget(context, element)
{
    if (!Strings::Equals(element.Name(), "Frame"))
    {
        DGEX_LOG_WARN("Constructing FrameWidget from non-Frame XML element: {}", element.Name());
    }
}

void FrameWidget::OnEvent(const Ref<Event>& event)
{
    DispatchEvent<WindowResizedEvent>(event, [this](const WindowResizedEvent& e) { return _OnWindowResized(e); });
    _Notify(event);
}

void FrameWidget::ApplyStyles()
{
    _properties.SetTransitionTime(GetStyleProperty<NumberProperty>("transition-time").Value);
    _properties.SetTransitionStyle(GetStyleProperty<StringProperty>("transition-style", StringProperty("none")).Value);

    _properties.SetPosition("auto");

    _properties.SetForegroundColor(GetStyleProperty<ColorProperty>("color", ColorProperty(Color::Black)).Value);
    _properties.SetBackgroundColor(
        GetStyleProperty<ColorProperty>("background-color", ColorProperty(Color::White)).Value);

    _properties.SetOpacity(
        Math::Clamp(GetStyleProperty<NumberProperty>("opacity", NumberProperty(1.0f)).Value, 0.0f, 1.0f));
    _properties.SetRotation(GetStyleProperty<NumberProperty>("rotation", NumberProperty(0.0f)).Value);
    _properties.SetScale(Math::ClampMin(GetStyleProperty<NumberProperty>("scale", NumberProperty(1.0f)).Value, 0.0f));

    _properties.SetFontSize(
        Math::ClampMin(GetStyleProperty<NumberProperty>("font-size", NumberProperty(16.0f)).Value, 1.0f));

    _properties.SetFont(
        GetStyleProperty<StringProperty>("font-family", StringProperty(GetDefaultFont()->GetName())).Value);
    _properties.SetFontStyle(GetStyleProperty<StringProperty>("font-style", StringProperty("Regular")).Value);

    _properties.SetTextAlign(GetStyleProperty<StringProperty>("text-align", StringProperty("left")).Value);
    _properties.SetVerticalAlign(GetStyleProperty<StringProperty>("vertical-align", StringProperty("top")).Value);

    // Recursively apply styles to children.
    for (const Ref<BaseWidget>& child : _children)
    {
        if (Ref<Widget> widget = child->AsWidget())
        {
            widget->ApplyStyles();
        }
    }
}

bool FrameWidget::_OnWindowResized(const WindowResizedEvent& event)
{
    _properties.SetWidth(static_cast<float>(event.GetWidth()));
    _properties.SetHeight(static_cast<float>(event.GetHeight()));
    return false;
}

} // namespace UI

DGEX_END
