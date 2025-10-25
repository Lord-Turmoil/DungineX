/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Widget.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 5, 2025                           *
 *                                                                            *
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Widget that can be displayed on the scree.                                 *
 ******************************************************************************/

#include <utility>

#include "DgeX/Application/UI/Widget/Widget.h"

#include "DgeX/Application/UI/Widget/WidgetContext.h"
#include "DgeX/Renderer/Texture.h"

DGEX_BEGIN

namespace UI
{

static void InitializeProperties(WidgetProperties& props)
{
    props.OffsetX = 0.0f;
    props.OffsetY = 0.0f;

    props.SetDisplay(DisplayValues::Block);
    props.SetPosition(PositionValues::Auto);

    props.SetX(0.0f);
    props.SetY(0.0f);
    props.SetWidth(0.0f);
    props.SetHeight(0.0f);

    props.UnSetForegroundColor();
    props.SetBackgroundColor(Color::White);

    props.SetOpacity(1.0f);
    props.SetRotation(0.0f);
    props.SetScale(1.0f);

    props.UnSetFontSize();
    props.UnSetFont();
    props.UnSetFontStyle();

    props.SetTextAlign(TextAlignValues::Left);
    props.SetVerticalAlign(VerticalAlignValues::Top);

    props.SetTransitionTime(0.0f);
    props.SetTransitionStyle("none");
}

Widget::Widget(std::string name, std::string id)
    : BaseWidget(std::move(name), std::move(id)), _texture(CreateTexture(0, 0))
{
    InitializeProperties(_properties);
}

Widget::Widget(const WidgetContext& context, Ext::XmlElement element)
    : BaseWidget(context, element), _texture(CreateTexture(0, 0))
{
    InitializeProperties(_properties);
}

Ptr<Widget> Widget::AsWidget()
{
    return this;
}

Ref<Widget> Widget::AsWidgetRef()
{
    return enable_shared_from_this<Widget>::shared_from_this();
}

Ref<Widget> Widget::ParentWidget() const
{
    if (const Ref<BaseWidget> parent = Parent())
    {
        return parent->AsWidgetRef();
    }
    return nullptr;
}

Ref<Widget> Widget::GetChildWidgetById(const std::string& id) const
{
    if (const Ref<BaseWidget> child = GetChildById(id))
    {
        return child->AsWidgetRef();
    }
    return nullptr;
}

void Widget::Update(DeltaTime delta)
{
    BaseWidget::Update(delta);
    _properties.Update(delta);
}

void Widget::ApplyStyles()
{
    _properties.SetTransitionTime(GetStyleProperty<NumberProperty>("transition-time").Value);
    _properties.SetTransitionStyle(GetStyleProperty<StringProperty>("transition-style", StringProperty("none")).Value);

    _properties.SetPosition(GetStyleProperty<PositionProperty>("position", PositionProperty()).Value);

    // Position and size will be set later during layout phase in `_Rearrange`.

    if (HasStyleProperty("color"))
    {
        _properties.SetForegroundColor(GetStyleProperty<ColorProperty>("color", ColorProperty(Color::Black)).Value);
    }
    else
    {
        _properties.UnSetForegroundColor();
    }
    _properties.SetBackgroundColor(
        GetStyleProperty<ColorProperty>("background-color", ColorProperty(Color::White)).Value);

    const NumberProperty opacity = GetStyleProperty<NumberProperty>("opacity", NumberProperty(1.0f));
    _properties.SetOpacity(Math::Clamp(opacity.Value, 0.0f, 1.0f));
    _properties.SetRotation(GetStyleProperty<NumberProperty>("rotation", NumberProperty(0.0f)).Value);
    _properties.SetScale(Math::ClampMin(GetStyleProperty<NumberProperty>("scale", NumberProperty(1.0f)).Value, 0.0f));

    if (HasStyleProperty("font-size"))
    {
        const NumberProperty fontSize = GetStyleProperty<NumberProperty>("font-size", NumberProperty(16.0f));
        _properties.SetFontSize(Math::ClampMin(fontSize.Value, 1.0f));
    }
    else
    {
        _properties.UnSetFontSize();
    }

    if (HasStyleProperty("font-family"))
    {
        _properties.SetFont(GetStyleProperty<StringProperty>("font-family").Value);
    }
    else
    {
        _properties.UnSetFont();
    }

    if (HasStyleProperty("font-style"))
    {
        _properties.SetFontStyle(GetStyleProperty<FontStyleProperty>("font-style").Value);
    }
    else
    {
        _properties.UnSetFontStyle();
    }

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

WidgetProperties& Widget::GetProperties()
{
    return _properties;
}

const WidgetProperties& Widget::GetProperties() const
{
    return _properties;
}

Ref<Texture> Widget::GetTexture() const
{
    return _texture;
}

void Widget::SetRenderCallback(const Ref<WidgetRenderCallback>& callback)
{
    _renderCallback = callback;
}

void Widget::Render(const WidgetRenderContext& context) const
{
    if (_renderCallback)
    {
        _renderCallback->Render(*this, context);
    }
}

bool Widget::_IsInside(FPoint position) const
{
    const float x = _properties.GlobalX();
    const float y = _properties.GlobalY();

    // clang-format off
    return (x < position.X) && (position.X < x + _properties.Width->Value()) &&
           (y < position.Y) && (position.Y < y + _properties.Height->Value());
    // clang-format on
}

void Widget::_ApplyWidth(const Widget& parent)
{
    const MetricProperty width = GetStyleProperty<MetricProperty>("width");
    if (width.Unit == MetricUnit::Pixel || width.Unit == MetricUnit::Unspecified)
    {
        _properties.SetWidth(Math::ClampMin(width.Value, 0.0f));
    }
    else if (width.Unit == MetricUnit::Percent)
    {
        _properties.SetWidth(Math::ClampMin(width.Value * 0.01f * parent.GetProperties().Width->Value(), 0.0f));
    }
}

void Widget::_ApplyHeight(const Widget& parent)
{
    const MetricProperty height = GetStyleProperty<MetricProperty>("height");
    if (height.Unit == MetricUnit::Pixel || height.Unit == MetricUnit::Unspecified)
    {
        _properties.SetHeight(Math::ClampMin(height.Value, 0.0f));
    }
    else if (height.Unit == MetricUnit::Percent)
    {
        _properties.SetHeight(Math::ClampMin(height.Value * 0.01f * parent.GetProperties().Height->Value(), 0.0f));
    }
}

/**
 * @brief Default rearrange.
 *
 * Default will rearrange auto-positioned children from top to bottom, left to right.
 */
void Widget::_Rearrange()
{
    float width = _properties.Width->Value();
    float cursorX = 0;
    float cursorY = 0;

    for (const Ref<BaseWidget>& child : _children)
    {
        const Ptr<Widget> widget = child->AsWidget();
        if (!widget)
        {
            continue;
        }

        WidgetProperties& props = widget->GetProperties();

        props.OffsetX = _properties.OffsetX + _properties.X->Value();
        props.OffsetY = _properties.OffsetY + _properties.Y->Value();

        widget->_ApplyWidth(*this);
        widget->_ApplyHeight(*this);

        if (props.Position->Value() == PositionValues::Relative)
        {
            // The X and Y we set here is relative to its parent, and will be adjusted to global
            // position during rearrangement. So, there we use ForceSet to avoid transition.
            const MetricProperty x = GetStyleProperty<MetricProperty>("x");
            if (x.Unit == MetricUnit::Pixel || x.Unit == MetricUnit::Unspecified)
            {
                props.SetX(x.Value);
            }
            else if (x.Unit == MetricUnit::Percent)
            {
                props.SetX(x.Value * _properties.Width->Value());
            }

            const MetricProperty y = GetStyleProperty<MetricProperty>("y");
            if (y.Unit == MetricUnit::Pixel || y.Unit == MetricUnit::Unspecified)
            {
                props.SetY(y.Value);
            }
            else if (y.Unit == MetricUnit::Percent)
            {
                props.SetY(y.Value * _properties.Height->Value());
            }
        }
        else /* auto */
        {
            props.SetX(cursorX);
            props.SetY(cursorY);

            float childWidth = props.Width->Value();
            float childHeight = props.Height->Value();
            if (cursorX + childWidth > width)
            {
                cursorX = 0;
                cursorY += childHeight;
            }
            cursorX += childWidth;
        }
    }
}

} // namespace UI

DGEX_END