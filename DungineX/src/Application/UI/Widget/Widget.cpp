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
 *                    Last Update : October 18, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Widget that can be displayed on the scree.                                 *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/Widget.h"

#include "DgeX/Application/UI/Style/Style.h"
#include "DgeX/Renderer/Texture.h"

DGEX_BEGIN

namespace UI
{

Widget::Widget(const WidgetContext& context, Ext::XmlElement element)
    : BaseWidget(context, element), _texture(CreateTexture(0, 0))
{
}

Ref<Widget> Widget::AsWidget()
{
    return shared_from_this();
}

Ref<Widget> Widget::ParentWidget() const
{
    if (const Ref<BaseWidget> parent = Parent())
    {
        return parent->AsWidget();
    }
    return nullptr;
}

Ref<Widget> Widget::GetChildWidgetById(const std::string& id) const
{
    if (const Ref<BaseWidget> child = GetChildById(id))
    {
        return child->AsWidget();
    }
    return nullptr;
}

void Widget::Update(DeltaTime delta)
{
    BaseWidget::Update(delta);
    _properties.Update(delta);
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

bool Widget::_IsInside(FPoint position) const
{
    // clang-format off
    return (_properties.X->Value() < position.X) &&
           (position.X < _properties.X->Value() + _properties.Width->Value()) &&
           (_properties.Y->Value() < position.Y) &&
           (position.Y < _properties.Y->Value() + _properties.Height->Value());
    // clang-format on
}

void Widget::_ApplyStyles()
{
    _properties.SetTransitionTime(GetStyleProperty<NumberProperty>("transition-time").Value);
    _properties.SetTransitionStyle(GetStyleProperty<StringProperty>("transition-style", StringProperty("none")).Value);

    _properties.SetPosition(GetStyleProperty<StringProperty>("position", StringProperty("auto")).Value);

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
        _properties.SetFontStyle(GetStyleProperty<StringProperty>("font-style").Value);
    }
    else
    {
        _properties.UnSetFontStyle();
    }

    _properties.SetTextAlign(GetStyleProperty<StringProperty>("text-align").Value);
    _properties.SetVerticalAlign(GetStyleProperty<StringProperty>("vertical-align").Value);

    // Recursively apply styles to children.
    for (const Ref<BaseWidget>& child : _children)
    {
        if (Ref<Widget> widget = child->AsWidget())
        {
            widget->_ApplyStyles();
        }
    }
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
    else
    {
        DGEX_CORE_WARN("Unsupported unit {} for width in widget '{}'", ToString(width.Unit), GetId());
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
    else
    {
        DGEX_CORE_WARN("Unsupported unit {} for height in widget '{}'", ToString(height.Unit), GetId());
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
        Ref<Widget> widget = child->AsWidget();
        if (!widget)
        {
            continue;
        }

        widget->_ApplyWidth(*this);
        widget->_ApplyHeight(*this);

        if (widget->GetProperties().Position->Value() == "relative")
        {
            // The X and Y we set here is relative to its parent, and will be adjusted to global
            // position during rearrangement. So, there we use ForceSet to avoid transition.
            const MetricProperty x = GetStyleProperty<MetricProperty>("x");
            if (x.Unit == MetricUnit::Pixel || x.Unit == MetricUnit::Unspecified)
            {
                _properties.SetX(x.Value);
            }
            else if (x.Unit == MetricUnit::Percent)
            {
                _properties.SetX(x.Value * _properties.Width->Value());
            }
            else
            {
                DGEX_CORE_WARN("Unsupported unit {} for x in widget '{}'", ToString(x.Unit), GetId());
            }

            const MetricProperty y = GetStyleProperty<MetricProperty>("y");
            if (y.Unit == MetricUnit::Pixel || y.Unit == MetricUnit::Unspecified)
            {
                _properties.SetY(y.Value);
            }
            else if (y.Unit == MetricUnit::Percent)
            {
                _properties.SetY(y.Value * _properties.Height->Value());
            }
            else
            {
                DGEX_CORE_WARN("Unsupported unit {} for y in widget '{}'", ToString(y.Unit), GetId());
            }
        }
        else /* auto */
        {
            widget->GetProperties().SetX(cursorX);
            widget->GetProperties().SetY(cursorY);

            float childWidth = widget->GetProperties().Width->Value();
            float childHeight = widget->GetProperties().Height->Value();
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
