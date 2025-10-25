/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetManager.cpp                         *
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
 * A manager wrapper to provide more controls over the widgets. It can allow  *
 * custom widgets to be loaded using factory pattern.                         *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/WidgetManager.h"

#include "DgeX/Application/UI/Widget/FrameWidget.h"
#include "DgeX/Application/UI/Widget/WidgetContext.h"
#include "DgeX/Utils/Strings.h"

DGEX_BEGIN

namespace UI
{

bool WidgetManager::Load(const WidgetContext& context, Ext::XmlElement element)
{
    if (_root)
    {
        DGEX_CORE_WARN("Already loaded, overwriting existing widgets.");
        _root.reset();
    }

    Ref<BaseWidget> widget = _Load(context, element);
    if (!widget)
    {
        DGEX_CORE_ERROR("Failed to load widget from XML element '{}'.", element.Name());
        return false;
    }

    Ref<FrameWidget> frame = widget->AsFrameWidgetRef();
    if (!frame)
    {
        DGEX_CORE_ERROR("Root widget must be a FrameWidget, got '{}'.", widget->GetName());
        return false;
    }

    _root = frame;

    return true;
}

Ref<Widget> WidgetManager::GetRootWidget() const
{
    return _root;
}

static void WidgetUpdate(const Ref<BaseWidget>& widget, DeltaTime delta)
{
    widget->Update(delta);

    for (const auto& child : widget->Children())
    {
        WidgetUpdate(child, delta);
    }
}

void WidgetManager::Update(DeltaTime delta) const
{
    if (!_root)
    {
        return;
    }

    WidgetUpdate(_root, delta);
}

static void WidgetOnEvent(Ptr<Widget> widget, Event& event)
{
    auto& children = widget->Children();

    // Reverse order to handle from z-index high to low.
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        if (const Ptr<Widget> child = (*it)->AsWidget())
        {
            WidgetOnEvent(child, event);
            if (event.IsHandled())
            {
                return;
            }
        }
    }

    widget->OnEvent(event);
}

void WidgetManager::OnEvent(Event& event) const
{
    if (!_root)
    {
        return;
    }

    WidgetOnEvent(_root.get(), event);
}

static void AddAllWidgets(std::vector<Ref<BaseWidget>>& widgets, const Ref<BaseWidget>& widget)
{
    widgets.push_back(widget);
    for (const Ref<BaseWidget>& child : widget->Children())
    {
        AddAllWidgets(widgets, child);
    }
}

Ref<BaseWidget> WidgetManager::_Load(const WidgetContext& context, Ext::XmlElement element)
{
    std::string name = element.Name();
    Ref<WidgetRegistration> registration = context.GetWidgetRegistration(name);
    if (!registration)
    {
        DGEX_LOG_WARN("Widget '{}' not recognized.", name);
        return nullptr;
    }

    Ref<BaseWidget> widget = registration->FactoryCallback(context, element);
    if (!widget)
    {
        DGEX_LOG_WARN("Failed to create widget '{}'.", name);
        return nullptr;
    }

    Ext::XmlElement child = element.FirstChild();
    while (child)
    {
        if (!Strings::Equals(child.Name(), "State"))
        {
            if (Ref<BaseWidget> childWidget = _Load(context, child))
            {
                widget->AddChild(childWidget);
            }
        }
        child = child.NextSibling();
    }

    return widget;
}

} // namespace UI

DGEX_END