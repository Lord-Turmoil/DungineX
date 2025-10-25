/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetContext.cpp                         *
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
 * Context during the loading of the widget. It includes styles, and maybe    *
 * also some metadata in the future. It does not hold resources, which are    *
 * handled by resource manager.                                               *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/WidgetContext.h"

#include "DgeX/Application/UI/Style/Style.h"
#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Log.h"

#include "Application/UI/Widget/WidgetImpl.h"

DGEX_BEGIN

namespace UI
{

Ref<WidgetRegistration> CreateWidgetRegistration(WidgetFactoryCallback factoryCallback,
                                                 Ref<WidgetRenderCallback> renderCallback)
{
    DGEX_ASSERT(factoryCallback, "Factory callback cannot be null");
    return CreateRef<WidgetRegistration>(WidgetRegistration{ std::move(factoryCallback), std::move(renderCallback) });
}

Ref<WidgetRegistration> CreateWidgetRegistration(WidgetFactoryCallback factoryCallback)
{
    DGEX_ASSERT(factoryCallback, "Factory callback cannot be null");
    return CreateRef<WidgetRegistration>(
        WidgetRegistration{ std::move(factoryCallback), CreateWidgetRenderCallback<Widget>(BasicWidgetRenderer()) });
}

void WidgetContext::AddStyle(const Ref<Style>& style)
{
    if (!style)
    {
        DGEX_CORE_WARN("Invalid style cannot be added to context");
        return;
    }

    if (_styles.find(style->GetName()) != _styles.end())
    {
        DGEX_CORE_WARN("Style with name '{}' already exists in context, replacing the old one", style->GetName());
    }

    _styles[style->GetName()] = style;
}

Ref<Style> WidgetContext::GetStyle(const std::string& name) const
{
    if (const auto it = _styles.find(name); it != _styles.end())
    {
        return it->second;
    }
    return nullptr;
}

void WidgetContext::RegisterWidget(const std::string& name, Ref<WidgetRegistration> registration)
{
    DGEX_ASSERT(registration, "Cannot register a null widget registration");
    _registeredWidgets[name] = std::move(registration);
}

Ref<WidgetRegistration> WidgetContext::GetWidgetRegistration(const std::string& name) const
{
    if (const auto it = _registeredWidgets.find(name); it != _registeredWidgets.end())
    {
        return it->second;
    }
    return nullptr;
}

} // namespace UI

DGEX_END