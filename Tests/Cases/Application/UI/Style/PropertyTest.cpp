#include "doctest/doctest.h"

#include <DgeX/DgeX.h>

TEST_CASE("Property Test")
{
    using namespace DgeX::UI;

    SUBCASE("NumberProperty")
    {
        CHECK_EQ(NumberProperty("50").Value, 50.0f);
        CHECK_EQ(NumberProperty("  -30.5  ").Value, -30.5f);
        CHECK_EQ(NumberProperty("+0.3%").Value, 0.003f);
        CHECK_EQ(NumberProperty("  100%  ").Value, 1.0f);
        CHECK_EQ(NumberProperty(".4").Value, 0.4f);
        CHECK_EQ(NumberProperty("5.6").Value, 5.6f);
    }

    SUBCASE("MetricProperty")
    {
        CHECK_EQ(MetricProperty("50px").Value, 50.0f);
        CHECK_EQ(MetricProperty("50px").Unit, MetricUnit::Pixel);
        CHECK_EQ(MetricProperty("  -30.5  s  ").Value, -30.5f);
        CHECK_EQ(MetricProperty("  -30.5  s  ").Unit, MetricUnit::Second);
        CHECK_EQ(MetricProperty("+0.3%").Value, 0.3f);
        CHECK_EQ(MetricProperty("+0.3%").Unit, MetricUnit::Percent);
        CHECK_EQ(MetricProperty("  100ms  ").Value, 100.0f);
        CHECK_EQ(MetricProperty("  100ms  ").Unit, MetricUnit::Millisecond);
    }

    SUBCASE("ColorProperty")
    {
        CHECK_EQ(ColorProperty("#AAA").Value, DgeX::Color("#AAAAAA"));
        CHECK_EQ(ColorProperty("#AAAA").Value, DgeX::Color("#AAAAAAAA"));
        CHECK_EQ(ColorProperty("#FF0000").Value, DgeX::Color(255, 0, 0));
        CHECK_EQ(ColorProperty("#00FF00").Value, DgeX::Color(0, 255, 0));
        CHECK_EQ(ColorProperty("#0000FF").Value, DgeX::Color(0, 0, 255));
        CHECK_EQ(ColorProperty("#80FFFFFF").Value, DgeX::Color(255, 255, 255, 128));
        CHECK_EQ(ColorProperty(" rgb(255, 0,0 ) ").Value, DgeX::Color(255, 0, 0));
        CHECK_EQ(ColorProperty(" rgba( 0, 255,0, 128) ").Value, DgeX::Color(0, 255, 0, 128));
    }
}
