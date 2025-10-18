/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetRenderer.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 11, 2025                          *
 *                                                                            *
 *                    Last Update : October 18, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Renderer for widgets. Since we use decoupled widget properties, all widgets*
 * can be rendered in a unified way.                                          *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/WidgetRenderer.h"

#include "DgeX/Application/UI/Widget/LabelWidget.h"
#include "DgeX/Application/UI/Widget/Widget.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Renderer/Texture.h"
#include "DgeX/Utils/Macros.h"

DGEX_BEGIN

namespace UI
{

static WidgetRenderContext GetChildWidgetContext(Widget& widget, const WidgetRenderContext& parentContext)
{
    WidgetRenderContext context;
    const WidgetProperties& properties = widget.GetProperties();

    context.Target = context.Canvas;
    if (properties.Font)
    {
        // TODO: Load font from resource manager.
        context.Font = parentContext.Font;
    }
    else
    {
        context.Font = parentContext.Font;
    }

    if (properties.FontStyle)
    {
        context.FontStyle = properties.FontStyle->Value();
    }
    else
    {
        context.FontStyle = parentContext.FontStyle;
    }

    if (properties.ForegroundColor)
    {
        context.FontColor = properties.ForegroundColor->Value();
    }
    else
    {
        context.FontColor = parentContext.FontColor;
    }

    if (widget.GetProperties().FontSize)
    {
        context.FontSize = properties.FontSize->Value();
    }
    else
    {
        context.FontSize = parentContext.FontSize;
    }

    return context;
}

void WidgetRenderer::Render(const Ref<Widget>& widget, WidgetRenderContext& context) const
{
    DGEX_ASSERT(widget, "Render null widget");

    const WidgetProperties& props = widget->GetProperties();
    context.Canvas = widget->GetTexture();

    // pre-render (set render target to the widget texture)
    float width = props.Width->Value();
    float height = props.Height->Value();
    if (static_cast<int>(width) != context.Canvas->GetWidth() ||
        static_cast<int>(height) != context.Canvas->GetHeight())
    {
        context.Canvas->Resize(static_cast<int>(width), static_cast<int>(height));
    }

    SetCurrentRenderTarget(context.Canvas);
    ClearDevice();

    // rendering (render to the widget's local texture)
    if (const auto it = _callbacks.find(widget->GetName()); it != _callbacks.end())
    {
        it->second->Render(widget, context);
    }
    for (const Ref<BaseWidget>& child : widget->Children())
    {
        if (const Ref<Widget> childWidget = child->AsWidget())
        {
            WidgetRenderContext childContext = GetChildWidgetContext(*childWidget, context);
            Render(childWidget, childContext);
        }
    }

    // post-render (render to parent render target)
    SetCurrentRenderTarget(context.Target);

    TextureStyle style;
    style.Scale = Math::ClampMin(props.Scale->Value(), 0.0f);
    style.Degree = props.Rotation->Value();
    style.Alpha = static_cast<uint8_t>(Math::Clamp(static_cast<int>(props.Opacity->Value()) * 255, 0, 255));

    DrawTexture(context.Canvas, style, static_cast<int>(props.X->Value()), static_cast<int>(props.Y->Value()));
}

void WidgetRenderer::AddCallback(const std::string& name, const Ref<WidgetRendererCallback>& callback)
{
    _callbacks[name] = callback;
}

void BasicWidgetRenderer::Render(const Ref<Widget>& widget, const WidgetRenderContext& context)
{
    const WidgetProperties& props = widget->GetProperties();
    SetFillColor(props.BackgroundColor->Value());
    DrawFilledRect(static_cast<int>(props.X->Value()), static_cast<int>(props.Y->Value()),
                   static_cast<int>(props.Width->Value()), static_cast<int>(props.Height->Value()));
}

void LabelWidgetRenderer::Render(const Ref<Widget>& widget, const WidgetRenderContext& context)
{
    Ref<LabelWidget> label = std::static_pointer_cast<LabelWidget>(widget);
    const WidgetProperties& props = label->GetProperties();

    SetFont(context.Font);
    SetFontStyle(context.FontStyle);
    SetFontSize(context.FontSize);
    SetFontColor(context.FontColor);

    TextFlags flags;
    const std::string& textAlign = props.TextAlign->Value();
    if (textAlign == "center")
    {
        flags = L(TextFlag::AlignCenter);
    }
    else if (textAlign == "right")
    {
        flags = L(TextFlag::AlignRight);
    }
    else
    {
        flags = L(TextFlag::AlignLeft);
    }

    Rect rect(static_cast<int>(props.X->Value()), static_cast<int>(props.Y->Value()),
              static_cast<int>(props.Width->Value()), static_cast<int>(props.Height->Value()));

    const std::string& verticalAlign = props.VerticalAlign->Value();
    if (verticalAlign == "middle")
    {
        Rect area = CalcTextArea(label->GetText().c_str(), rect, flags);
        area.Y = rect.Y + (rect.Height - area.Height) / 2;
        DrawTextArea(label->GetText().c_str(), area, flags);
    }
    else if (verticalAlign == "bottom")
    {
        Rect area = CalcTextArea(label->GetText().c_str(), rect, flags);
        area.Y = rect.Y + rect.Height - area.Height;
        DrawTextArea(label->GetText().c_str(), area, flags);
    }
    else
    {
        DrawTextArea(label->GetText().c_str(), rect, flags);
    }
}

} // namespace UI

DGEX_END
