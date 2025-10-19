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
 *                    Last Update : October 19, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * A manager wrapper to provide more controls over the widgets. It can allow  *
 * custom widgets to be loaded using factory pattern.                         *
 ******************************************************************************/

#include "DgeX/Application/UI/Widget/WidgetManager.h"

#include "DgeX/Application/UI/Widget/FrameWidget.h"
#include "DgeX/Utils/Strings.h"

#include <algorithm>

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

    Ref<FrameWidget> frame = widget->AsFrameWidget();
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

void WidgetManager::RegisterWidget(const std::string& name, const WidgetFactoryCallback& callback)
{
    if (const auto it = _callbacks.find(name); it != _callbacks.end())
    {
        DGEX_CORE_WARN("Overriding existing widget factory for widget type '{}'.", name);
    }
    _callbacks[name] = callback;
}

void WidgetManager::Update(DeltaTime delta) const
{
    if (_root)
    {
        _root->Update(delta);
    }
}

void WidgetManager::OnEvent(const Ref<Event>& event) const
{
    for (const Ref<BaseWidget>& widget : _widgets)
    {
        widget->OnEvent(event);
        if (event->IsHandled())
        {
            break;
        }
    }
}

static void AddAllWidgets(std::vector<Ref<BaseWidget>>& widgets, const Ref<BaseWidget>& widget)
{
    widgets.push_back(widget);
    for (const Ref<BaseWidget>& child : widget->Children())
    {
        AddAllWidgets(widgets, child);
    }
}

void WidgetManager::Reorder()
{
    _widgets.clear();
    if (!_root)
    {
        return;
    }

    AddAllWidgets(_widgets, _root);

    // Sort by z-index from high to low, if equal, sort by level from high to low.
    std::sort(_widgets.begin(), _widgets.end(), [](const Ref<BaseWidget>& lhs, const Ref<BaseWidget>& rhs) {
        if (lhs->GetZIndex() != rhs->GetZIndex())
        {
            return lhs->GetZIndex() > rhs->GetZIndex();
        }
        return lhs->GetLevel() > rhs->GetLevel();
    });
}

Ref<BaseWidget> WidgetManager::_Load(const WidgetContext& context, Ext::XmlElement element)
{
    std::string name = element.Name();
    auto it = _callbacks.find(name);
    if (it == _callbacks.end())
    {
        DGEX_LOG_WARN("No widget factory registered for widget type '{}'.", name);
        return nullptr;
    }

    Ref<BaseWidget> widget = it->second(context, element);
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
