#include <DgeX/DgeX.h>

#include "doctestplus.h"

TEST_CASE("Property Parse Test")
{
    const char* value;

    SUBCASE("Color")
    {
        DgeX::ColorProperty property;

        value = "#FF0000";
        property.Apply(value);
        CHECK_EQ(property.GetColor().ToUInt32(), 0xFFFF0000);

        value = "#33FF9944";
        property.Apply(value);
        CHECK_EQ(property.GetColor().ToUInt32(), 0x33FF9944);

        value = "rgb(255, 0, 0)";
        property.Apply(value);
        CHECK_EQ(property.GetColor().ToUInt32(), 0xFFFF0000);

        value = "rgba(16, 32, 64, 255)";
        property.Apply(value);
        CHECK_EQ(property.GetColor().ToUInt32(), 0xFF102040);
    }

    SUBCASE("Size")
    {
        DgeX::SizeProperty property;

        value = "10px 20px";
        property.Apply(value);
        CHECK_EQ(property.GetWidth().ToPixel(200), 10);
        CHECK_EQ(property.GetHeight().ToPixel(200), 20);

        value = "50% 60%";
        property.Apply(value);
        CHECK_EQ(property.GetWidth().ToPixel(200), 100);
        CHECK_EQ(property.GetHeight().ToPixel(200), 120);

        value = "30px 70%";
        property.Apply(value);
        CHECK_EQ(property.GetWidth().ToPixel(200), 30);
        CHECK_EQ(property.GetHeight().ToPixel(200), 140);

        value = "80 90";
        property.Apply(value);
        CHECK_EQ(property.GetWidth().ToPixel(200), 80);
        CHECK_EQ(property.GetHeight().ToPixel(200), 90);
    }
}

TEST_CASE("Property Change Test")
{
    DgeX::PropertyGroup group;

    group.AddProperty<DgeX::OpacityProperty>()->OnOpacityChange([](float& oldValue, const float& newValue) {
        // Only change if opacity is greater than 0.5.
        if (newValue > 0.5f)
        {
            oldValue = newValue;
        }
    });

    auto opacity = group.GetProperty<DgeX::OpacityProperty>();
    CHECK_NE(opacity, nullptr);

    CHECK_EQ(opacity->GetOpacity(), 1.0f);
    opacity->SetOpacity(0.3f);
    CHECK_EQ(opacity->GetOpacity(), 1.0f);
    opacity->SetOpacity(0.8f);
    CHECK_EQ(opacity->GetOpacity(), 0.8f);

    // sneakily change opacity
    opacity->SetOpacitySilent(0.3f);
    CHECK_EQ(opacity->GetOpacity(), 0.3f);
}
