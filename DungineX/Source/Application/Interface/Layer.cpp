#include "DgeX/Application/Interface/Layer.h"
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

DGEX_END
