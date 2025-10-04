/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : VisualWidget.h                            *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 4, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Widget that can be displayed on the scree.                                 *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/UI/Style/WidgetProperty.h"
#include "DgeX/Application/UI/Widget/BaseWidget.h"

DGEX_BEGIN

namespace UI
{

/**
 * @brief Base class for visible widgets.
 *
 * Intentionally private inherit from std::enable_shared_from_this.
 */
class Widget : public BaseWidget, std::enable_shared_from_this<Widget>
{
public:
    Widget(Ext::XmlElement element);
    ~Widget() override = default;

    Ref<Widget> AsWidget() override;

    Ref<Widget> ParentWidget() const;
    Ref<Widget> GetChildWidgetById(const std::string& id) const;

public:
    void Update(DeltaTime delta) override;

    WidgetProperties& GetProperties();
    const WidgetProperties& GetProperties() const;

protected:
    bool _IsInside(FPoint position) const override;
    void _ApplyStyles() override;

    /**
     * @brief Rearrange the widget layout.
     */
    virtual void _Rearrange();

private:
    WidgetProperties _properties;
};

} // namespace UI

DGEX_END
