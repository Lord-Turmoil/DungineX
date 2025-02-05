#pragma once

#include "DgeX/Application/Widget/EventListener.h"
#include "DgeX/Core/Base.h"
#include "DgeX/Core/DeltaTime.h"

#include <string>
#include <unordered_map>

DGEX_BEGIN

class PropertyGroup;
class EventListener;

class DomElement;
class DomContext;

/**
 * @brief Both DomElement and DomContext derive from DomNode.
 * It contains some fundamental properties.
 */
class DomNode
{
public:
    DomNode();

    DomNode(const DomNode&) = delete;
    DomNode(DomNode&& other) noexcept = default;
    DomNode& operator=(const DomNode&) = delete;
    DomNode& operator=(DomNode&& other) noexcept = default;

    virtual ~DomNode() = default;

    virtual Ptr<DomContext> AsContext();
    virtual Ptr<DomElement> AsElement();

public:
    virtual void OnUpdate(DeltaTime delta) = 0;
    virtual void OnEvent(const Ref<Event>& event) = 0;
    virtual void OnRender() = 0;

    virtual void ComputeProperties() = 0;
    virtual void ComputeLayout() = 0;

public:
    Ptr<DomContext> GetContext() const;

    Ptr<DomNode> GetParentNode() const;
    Ptr<DomElement> GetParentElement() const;

    Ref<PropertyGroup> GetProperties() const;

    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;

    float GetParentWidth() const;
    float GetParentHeight() const;

    void Resize(float width, float height);

protected:
    Ptr<DomContext> _context;

    float _x;
    float _y;
    float _width;
    float _height;

    Ptr<DomNode> _parent;
    std::vector<Ref<DomElement>> _children;

    Ref<PropertyGroup> _properties;
};

DGEX_END
