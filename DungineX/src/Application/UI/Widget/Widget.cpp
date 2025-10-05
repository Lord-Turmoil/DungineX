/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : VisualWidget.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 5, 2025                           *
 *                                                                            *
 *                    Last Update : October 5, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Widget that can be displayed on the scree.                                 *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/Widget.h"

#include "DgeX/Application/UI/Style/Style.h"

DGEX_BEGIN

namespace UI
{

Widget::Widget(Ext::XmlElement element)
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
    const Ref<Widget> parent = ParentWidget();
    if (!parent)
    {
        DGEX_CORE_WARN("Widget '{}' has no parent yet, cannot apply styles", GetId());
        return;
    }

    _properties.SetTransitionTime(GetStyleProperty<NumberProperty>("transitionTime").Value);
    _properties.SetTransitionStyle(GetStyleProperty<StringProperty>("transitionStyle", StringProperty("none")).Value);

    const StringProperty position = GetStyleProperty<StringProperty>("position", StringProperty("auto"));
    if (position.Value == "absolute")
    {
        const MetricProperty x = GetStyleProperty<MetricProperty>("x");
        if (x.Unit == MetricUnit::Pixel || x.Unit == MetricUnit::Unspecified)
        {
            _properties.SetX(x.Value);
        }
        else if (x.Unit == MetricUnit::Percent)
        {
            _properties.SetX(x.Value * parent->GetProperties().Width->Value());
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
            _properties.SetY(y.Value * parent->GetProperties().Height->Value());
        }
        else
        {
            DGEX_CORE_WARN("Unsupported unit {} for y in widget '{}'", ToString(y.Unit), GetId());
        }
    }

    const MetricProperty width = GetStyleProperty<MetricProperty>("width");
    if (width.Unit == MetricUnit::Pixel || width.Unit == MetricUnit::Unspecified)
    {
        _properties.SetWidth(Math::ClampMin(width.Value, 0.0f));
    }
    else if (width.Unit == MetricUnit::Percent)
    {
        _properties.SetWidth(Math::ClampMin(width.Value * parent->GetProperties().Width->Value(), 0.0f));
    }
    else
    {
        DGEX_CORE_WARN("Unsupported unit {} for width in widget '{}'", ToString(width.Unit), GetId());
    }
    const MetricProperty height = GetStyleProperty<MetricProperty>("height");
    if (height.Unit == MetricUnit::Pixel || height.Unit == MetricUnit::Unspecified)
    {
        _properties.SetHeight(Math::ClampMin(height.Value, 0.0f));
    }
    else if (height.Unit == MetricUnit::Percent)
    {
        _properties.SetHeight(Math::ClampMin(height.Value * parent->GetProperties().Height->Value(), 0.0f));
    }
    else
    {
        DGEX_CORE_WARN("Unsupported unit {} for height in widget '{}'", ToString(height.Unit), GetId());
    }

    _properties.SetForegroundColor(GetStyleProperty<ColorProperty>("color", ColorProperty(Color::Black)).Value);
    _properties.SetBackgroundColor(
        GetStyleProperty<ColorProperty>("backgroundColor", ColorProperty(Color::White)).Value);

    const NumberProperty opacity = GetStyleProperty<NumberProperty>("opacity", NumberProperty(1.0f));
    _properties.SetOpacity(Math::Clamp(opacity.Value, 0.0f, 1.0f));
}

} // namespace UI

DGEX_END
