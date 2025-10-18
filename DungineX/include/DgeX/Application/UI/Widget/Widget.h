/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Widget.h                                  *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 11, 2025                          *
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

class Texture;

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
    explicit Widget(const WidgetContext& context, Ext::XmlElement element);
    ~Widget() override = default;

    DGEX_API Ref<Widget> AsWidget() override;

    DGEX_API Ref<Widget> ParentWidget() const;
    DGEX_API Ref<Widget> GetChildWidgetById(const std::string& id) const;

public:
    void Update(DeltaTime delta) override;

    WidgetProperties& GetProperties();
    const WidgetProperties& GetProperties() const;

    Ref<Texture> GetTexture() const;

protected:
    bool _IsInside(FPoint position) const override;
    void _ApplyStyles() override;

    // Some properties may have different default values, so we make these apply
    // methods virtual for derived widgets to override.
    virtual void _ApplyWidth(const Widget& parent);
    virtual void _ApplyHeight(const Widget& parent);

    /**
     * @brief Rearrange the widget layout.
     */
    virtual void _Rearrange();

protected:
    WidgetProperties _properties;

private:
    Ref<Texture> _texture;
};

} // namespace UI

DGEX_END
