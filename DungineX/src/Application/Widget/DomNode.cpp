#include "DgeX/Application/Widget/DomNode.h"

#include "DgeX/Application/Style/Property/Property.h"
#include "DgeX/Application/Style/Property/PropertyGroup.h"
#include "DgeX/Application/Style/Stylesheet.h"
#include "DgeX/Application/Widget/DomContext.h"
#include "DgeX/Application/Widget/DomElement.h"

#include "DgeX/Core/Assert.h"

DGEX_BEGIN

DomNode::DomNode()
    : _context(nullptr), _x(0.0f), _y(0.0f), _width(0.0f), _height(0.0f), _parent(nullptr),
      _properties(CreateRef<PropertyGroup>())
{
}

Ptr<DomContext> DomNode::AsContext()
{
    return nullptr;
}

Ptr<DomElement> DomNode::AsElement()
{
    return nullptr;
}

Ptr<DomContext> DomNode::GetContext() const
{
    return _context;
}

Ptr<DomNode> DomNode::GetParentNode() const
{
    return _parent;
}

Ptr<DomElement> DomNode::GetParentElement() const
{
    return _parent ? _parent->AsElement() : nullptr;
}

Ref<PropertyGroup> DomNode::GetProperties() const
{
    return _properties;
}

float DomNode::GetX() const
{
    return _x;
}

float DomNode::GetY() const
{
    return _y;
}

float DomNode::GetWidth() const
{
    return _width;
}

float DomNode::GetHeight() const
{
    return _height;
}

float DomNode::GetParentWidth() const
{
    DGEX_ASSERT(_context);
    return _parent ? _parent->_width : _context->GetWidth();
}

float DomNode::GetParentHeight() const
{
    DGEX_ASSERT(_context);
    return _parent ? _parent->_height : _context->GetHeight();
}

void DomNode::Resize(float width, float height)
{
    _width = width;
    _height = height;
    ComputeLayout();
}

DGEX_END
