#include <DgeX/DgeX.h>

#include "doctestplus.h"

TEST_CASE("DOM Parse Test")
{
    const char* filename = "Resource/Page.xml";
    DgeX::DomContext context;
    DgeX::Ref<DgeX::DomElement> element;

    CHECK_FALSE(context.Load("Resource/NotExists.xml"));

    REQUIRE(context.Load(filename));
    auto page = context.GetRootElement();

    SUBCASE("Structure Test")
    {
        element = page->FirstChild();
        REQUIRE(element);
        CHECK_EQ(element->TagName(), "div");
        CHECK(element->HasClass("wrapper"));

        element = element->FirstChild();
        REQUIRE(element);
        CHECK_EQ(element->TagName(), "div");
        CHECK(element->HasClass("header"));

        element = element->FirstChild();
        REQUIRE(element);
        CHECK_EQ(element->TagName(), "Label");
        CHECK_EQ(element->Id(), "header");
        CHECK(element->HasClass("label"));
        CHECK(element->HasClass("big"));

        element = element->GetParentElement()->NextSibling();
        REQUIRE(element);
        CHECK_EQ(element->TagName(), "div");
        CHECK(element->HasClass("footer"));

        element = element->FirstChild();
        REQUIRE(element);
        CHECK_EQ(element->TagName(), "Label");
        CHECK_EQ(element->Id(), "footer");
        CHECK(element->HasClass("label"));
        CHECK(element->HasClass("small"));
    }

    SUBCASE("Style Test")
    {
        element = page->GetElementById("header");
        CHECK_EQ(element->GetProperties()->GetProperty<DgeX::ColorProperty>()->GetColor().ToUInt32(), 0xFFFF00FF);

        element->SetState(DgeX::DomElementState::Hover);
        CHECK_EQ(element->GetProperties()->GetProperty<DgeX::ColorProperty>()->GetColor().ToUInt32(), 0xFFFFFF00);
    }
}
