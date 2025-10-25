/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetImpl.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 19, 2025                          *
 *                                                                            *
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Built-in widget implementation.                                            *
 ******************************************************************************/

#include "Application/UI/Widget/WidgetImpl.h"

#include "DgeX/Application/UI/Widget/LabelWidget.h"
#include "DgeX/Application/UI/Widget/Widget.h"
#include "DgeX/Application/UI/Widget/WidgetContext.h"
#include "DgeX/Renderer/RenderApi.h"

DGEX_BEGIN

namespace UI
{

Ref<BaseWidget> BasicWidgetFactory::operator()(const WidgetContext& context, const Ext::XmlElement& element) const
{
    return CreateRef<Widget>(context, element);
}

void BasicWidgetRenderer::operator()(const Widget& widget, const WidgetRenderContext& context) const
{
    DGEX_USED(context);

    const WidgetProperties& props = widget.GetProperties();
    SetFillColor(props.BackgroundColor->Value());
    DrawFilledRect(static_cast<int>(props.X->Value()), static_cast<int>(props.Y->Value()),
                   static_cast<int>(props.Width->Value()), static_cast<int>(props.Height->Value()));
}

Ref<BaseWidget> LabelWidgetFactory::operator()(const WidgetContext& context, const Ext::XmlElement& element) const
{
    return CreateRef<LabelWidget>(context, element);
}

void LabelWidgetRenderer::operator()(const LabelWidget& widget, const WidgetRenderContext& context) const
{
    const WidgetProperties& props = widget.GetProperties();

    SetFont(context.Font);
    SetFontStyle(context.FontStyle);
    SetFontSize(context.FontSize);
    SetFontColor(context.FontColor);

    TextFlags flags;
    TextAlignValues textAlign = props.TextAlign->Value();
    if (textAlign == TextAlignValues::Center)
    {
        flags = L(TextFlag::AlignCenter);
    }
    else if (textAlign == TextAlignValues::Right)
    {
        flags = L(TextFlag::AlignRight);
    }
    else
    {
        flags = L(TextFlag::AlignLeft);
    }

    Rect rect(static_cast<int>(props.X->Value()), static_cast<int>(props.Y->Value()),
              static_cast<int>(props.Width->Value()), static_cast<int>(props.Height->Value()));

    VerticalAlignValues verticalAlign = props.VerticalAlign->Value();
    if (verticalAlign == VerticalAlignValues::Middle)
    {
        Rect area = CalcTextArea(widget.GetText().c_str(), rect, flags);
        area.Y = rect.Y + (rect.Height - area.Height) / 2;
        DrawTextArea(widget.GetText().c_str(), area, flags);
    }
    else if (verticalAlign == VerticalAlignValues::Bottom)
    {
        Rect area = CalcTextArea(widget.GetText().c_str(), rect, flags);
        area.Y = rect.Y + rect.Height - area.Height;
        DrawTextArea(widget.GetText().c_str(), area, flags);
    }
    else
    {
        DrawTextArea(widget.GetText().c_str(), rect, flags);
    }
}

} // namespace UI

DGEX_END