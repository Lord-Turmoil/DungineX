/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetRenderer.h                          *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 11, 2025                          *
 *                                                                            *
 *                    Last Update : October 11, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Renderer for widgets. Since we use decoupled widget properties, all widgets*
 * can be rendered in a unified way.                                          *
 ******************************************************************************/

#pragma once

#include "DgeX/Utils/Types.h"

#include <string>
#include <unordered_map>

DGEX_BEGIN

class Texture;

namespace UI
{

class Widget;

/**
 * @brief Renderer callback for a specific widget type.
 */
class WidgetRendererCallback
{
public:
    WidgetRendererCallback() = default;
    virtual ~WidgetRendererCallback() = default;

    /**
     * @brief Should call _PreRender before rendering, and _PostRender after rendering.
     *
     * @param widget Widget to render.
     */
    virtual void Render(const Ref<Widget>& widget) = 0;
};

/**
 * @brief A de-coupled widget renderer.
 */
class WidgetRenderer final
{
public:
    WidgetRenderer() = default;
    ~WidgetRenderer() = default;

    void Render(const Ref<Widget>& widget) const;

    void AddCallback(const std::string& name, const Ref<WidgetRendererCallback>& callback);

private:
    std::unordered_map<std::string, Ref<WidgetRendererCallback>> _callbacks;
};

class BasicWidgetRenderer : public WidgetRendererCallback
{
public:
    void Render(const Ref<Widget>& widget) override;
};

class LabelWidgetRenderer : public WidgetRendererCallback
{
public:
    void Render(const Ref<Widget>& widget) override;
};

} // namespace UI

DGEX_END
