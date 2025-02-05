#include <DgeX/DgeX.h>

#include "doctestplus.h"

TEST_CASE("Selector Parse Test")
{
    const char* value;
    DgeX::Selector selector;

    SUBCASE("Normal")
    {
        value = "div.class1:hover.class2:active .out.in";
        selector.Assign(value);
        CHECK_EQ(selector.ToString(), value);

        value = "  #id1  div.wrapper:normal  .a.b.c:hover #id2:active  ";
        selector.Assign(value);
        CHECK_EQ(selector.ToString(), "#id1 div.wrapper .a.b.c:hover #id2:active");
    }

    SUBCASE("Undefined behavior")
    {
        value = "  div .class1 :hover .class2 :active  .out .in  ";
        selector.Assign(value);
        CHECK_EQ(selector.ToString(), "div .class1 hover .class2 active .out .in");
    }
}

TEST_CASE("Selector Match Test")
{
    const char* filename = "Resource/Page.xml";
    DgeX::DomContext context;
    DgeX::Ref<DgeX::DomElement> element;
    DgeX::Selector selector;

    REQUIRE(context.Load(filename));
    auto page = context.GetRootElement();

    element = page->GetElementById("header");
    CHECK_NON_NULL(element);
    selector.Assign("#header");
    CHECK(selector.Match(*element));
    selector.Assign("div.wrapper div.header Label");
    CHECK(selector.Match(*element));

    selector.Assign(".wrapper .big");
    CHECK(selector.Match(*element));
    selector.Assign(".wrapper .small");
    CHECK_FALSE(selector.Match(*element));

    selector.Assign("div div Label:hover");
    CHECK_FALSE(selector.Match(*element));
    element->SetState(DgeX::DomElementState::Hover);
    CHECK(selector.Match(*element));
}
