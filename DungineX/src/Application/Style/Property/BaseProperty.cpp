#include "DgeX/Application/Style/Property/BaseProperty.h"

DGEX_BEGIN

void BaseProperty::Load(const Ref<Style>& style)
{
    if (const char* value = style->GetProperty(Name()))
    {
        Apply(value);
    }
}

DGEX_END
