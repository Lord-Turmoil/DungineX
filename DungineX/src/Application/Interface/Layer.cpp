#include "DgeX/Application/Interface/Layer.h"
#include "DgeX/Application/Interface/Interface.h"
#include "DgeX/Core/UUID.h"

DGEX_BEGIN

Layer::Layer(const std::string& name) : _parent(nullptr)
{
    if (name.empty())
    {
        _name = "Layer ";
        _name += UUID().ToString();
    }
    else
    {
        _name = name;
    }
}

int Layer::GetWidth() const
{
    return _parent ? _parent->GetWidth() : 0;
}

int Layer::GetHeight() const
{
    return _parent ? _parent->GetHeight() : 0;
}

DGEX_END
