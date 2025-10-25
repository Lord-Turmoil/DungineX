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
 *                    Last Update : October 25, 2025                          *
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

Ptr<FrameWidget> FrameWidget::AsFrameWidget()
{
    return this;
}

Ref<FrameWidget> FrameWidget::AsFrameWidgetRef()
{
    return enable_shared_from_this<FrameWidget>::shared_from_this();
}

void FrameWidget::OnEvent(Event& event)
{
    DispatchEvent<WindowResizedEvent>(event, [this](const WindowResizedEvent& e) { return _OnWindowResized(e); });
    _Notify(event);
}

void FrameWidget::ApplyStyles()
{
    _properties.SetTransitionTime(GetStyleProperty<NumberProperty>("transition-time").Value);
    _properties.SetTransitionStyle(GetStyleProperty<StringProperty>("transition-style", StringProperty("none")).Value);

    _properties.SetPosition(PositionValues::Auto);

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
    _properties.SetFontStyle(GetStyleProperty<FontStyleProperty>("font-style").Value);

    _properties.SetTextAlign(GetStyleProperty<TextAlignProperty>("text-align").Value);
    _properties.SetVerticalAlign(GetStyleProperty<VerticalAlignProperty>("vertical-align").Value);

    // Recursively apply styles to children.
    for (const Ref<BaseWidget>& child : _children)
    {
        if (const Ptr<Widget> widget = child->AsWidget())
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