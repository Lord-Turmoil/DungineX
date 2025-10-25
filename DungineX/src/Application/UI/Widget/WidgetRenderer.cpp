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
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Renderer for widgets. Since we use decoupled widget properties, all widgets*
 * can be rendered in a unified way.                                          *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/WidgetRenderer.h"

#include "DgeX/Application/UI/Widget/Widget.h"
#include "DgeX/Application/UI/Widget/WidgetContext.h"
#include "DgeX/Renderer/Font.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Renderer/Texture.h"

DGEX_BEGIN

namespace UI
{

static WidgetRenderContext GetRootContext(const Widget& widget);
static WidgetRenderContext GetChildContext(const Widget& widget, const WidgetRenderContext& parentContext);

static void PreRender(WidgetRenderContext& context, const Widget& widget);
static void PostRender(const WidgetRenderContext& context, const Widget& widget);

void WidgetRenderer::Render(const Widget& widget) const
{
    WidgetRenderContext context = GetRootContext(widget);

    Render(widget, context);
}

void WidgetRenderer::Render(const Widget& widget, WidgetRenderContext& context) const
{
    PreRender(context, widget);

    // Render parent widget first.
    widget.Render(context);

    // Children are ordered by z-index from low to high, so we can just render them in order.
    for (const Ref<BaseWidget>& child : widget.Children())
    {
        if (const Ptr<Widget> childWidget = child->AsWidget())
        {
            WidgetRenderContext childContext = GetChildContext(*childWidget, context);
            Render(*childWidget, childContext);
        }
    }

    PostRender(context, widget);
}

// ============================================================================
// Internal function implementation.
// ----------------------------------------------------------------------------

WidgetRenderContext GetRootContext(const Widget& widget)
{
    const WidgetProperties& props = widget.GetProperties();
    WidgetRenderContext context;

    context.Target = GetCurrentRenderTarget();
    context.Font = GetDefaultFont(); // TODO: Load font form resource manager.
    context.FontStyle = props.FontStyle->Value();
    context.FontColor = props.ForegroundColor->Value();
    context.FontSize = props.FontSize->Value();

    return context;
}

WidgetRenderContext GetChildContext(const Widget& widget, const WidgetRenderContext& parentContext)
{
    const WidgetProperties& properties = widget.GetProperties();
    WidgetRenderContext context;

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

void PreRender(WidgetRenderContext& context, const Widget& widget)
{
    const WidgetProperties& props = widget.GetProperties();
    context.Canvas = widget.GetTexture();

    float width = props.Width->Value();
    float height = props.Height->Value();
    if (static_cast<int>(width) != context.Canvas->GetWidth() ||
        static_cast<int>(height) != context.Canvas->GetHeight())
    {
        context.Canvas->Resize(static_cast<int>(width), static_cast<int>(height));
    }

    SetCurrentRenderTarget(context.Canvas);
    ClearDevice();
}

void PostRender(const WidgetRenderContext& context, const Widget& widget)
{
    const WidgetProperties& props = widget.GetProperties();

    SetCurrentRenderTarget(context.Target);

    TextureStyle style;
    style.Scale = Math::ClampMin(props.Scale->Value(), 0.0f);
    style.Degree = props.Rotation->Value();
    style.Alpha = static_cast<uint8_t>(Math::Clamp(static_cast<int>(props.Opacity->Value()) * 255, 0, 255));

    DrawTexture(context.Canvas, style, static_cast<int>(props.X->Value()), static_cast<int>(props.Y->Value()));
}

} // namespace UI

DGEX_END