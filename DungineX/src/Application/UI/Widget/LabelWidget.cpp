/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : LabelWidget.h                             *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 12, 2025                          *
 *                                                                            *
 *                    Last Update : October 12, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Label widget has text.                                                     *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/LabelWidget.h"

DGEX_BEGIN

namespace UI
{

LabelWidget::LabelWidget(WidgetContext& context, const Ext::XmlElement& element) : Widget(context, element)
{
    _text = element.Attribute("text", "");
}

void LabelWidget::SetText(const std::string& text)
{
    _text = text;
}

const std::string& LabelWidget::GetText() const
{
    return _text;
}

void LabelWidget::_ApplyWidth(const Widget& parent)
{
    const MetricProperty width = GetStyleProperty<MetricProperty>("width", MetricProperty(100.0f, MetricUnit::Percent));
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

void LabelWidget::_ApplyHeight(const Widget& parent)
{
    const MetricProperty height =
        GetStyleProperty<MetricProperty>("height", MetricProperty(100.0f, MetricUnit::Percent));
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

} // namespace UI

DGEX_END
