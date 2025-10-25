/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetImpl.h                              *
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

#pragma once

#include "DgeX/Application/UI/Widget/BaseWidget.h"
#include "DgeX/Application/UI/Widget/WidgetRenderer.h"
#include "DgeX/Extension/XmlDocument.h"

DGEX_BEGIN

namespace UI
{

// ============================================================================
// General Widget
// - <Box>
// - <Button>
// - <Frame>
// ----------------------------------------------------------------------------

struct BasicWidgetFactory
{
    Ref<BaseWidget> operator()(const WidgetContext& context, const Ext::XmlElement& element) const;
};

struct BasicWidgetRenderer
{
    void operator()(const Widget& widget, const WidgetRenderContext& context) const;
};

// ============================================================================
// Label Widget
// - <Label>
// ----------------------------------------------------------------------------

struct LabelWidgetFactory
{
    Ref<BaseWidget> operator()(const WidgetContext& context, const Ext::XmlElement& element) const;
};

struct LabelWidgetRenderer
{
    void operator()(const LabelWidget& widget, const WidgetRenderContext& context) const;
};

} // namespace UI

DGEX_END