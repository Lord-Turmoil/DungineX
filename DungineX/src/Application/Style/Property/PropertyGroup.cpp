#include "DgeX/Application/Style/Property/PropertyGroup.h"

DGEX_BEGIN

void PropertyGroup::Load(const tinyxml2::XMLElement& element) const
{
    for (auto& it : _properties)
    {
        it.second->Load(element);
    }
}

void PropertyGroup::Load(const Ref<Style>& style) const
{
    for (auto& it : _properties)
    {
        it.second->Load(style);
    }
}

DGEX_END
