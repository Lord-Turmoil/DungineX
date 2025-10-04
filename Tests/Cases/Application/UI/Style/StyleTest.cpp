#include "DgeX/Application/UI/Style/Style.h"

#include "doctest/Common.h"
#include "doctest/doctest.h"

#include <DgeX/DgeX.h>

TEST_CASE("Style Load Test")
{
    using namespace DgeX;
    using namespace DgeX::UI;

    const auto FILE_PATH = GetResource("Extension", "XmlDocumentTest.xml");
    Ext::XmlDocument doc(FILE_PATH.string());

    Ext::XmlElement stylesElement = doc.GetRoot().FirstChild();
    REQUIRE(stylesElement.IsValid());

    Ext::XmlElement boxStyleElement = stylesElement.FirstChild();
    REQUIRE(boxStyleElement.IsValid());
    Ref<Style> boxStyle = CreateRef<Style>(boxStyleElement);
    CHECK_EQ(boxStyle->GetProperty("width"), "200px");
    CHECK_EQ(boxStyle->GetProperty("height"), "100px");

    Ext::XmlElement buttonStyleElement = boxStyleElement.NextSibling();
    REQUIRE(buttonStyleElement.IsValid());
    Ref<Style> buttonStyle = CreateRef<Style>(buttonStyleElement);
    CHECK_EQ(buttonStyle->GetProperty("fontSize"), "20px");
    CHECK_EQ(buttonStyle->GetStateProperty("hover", "fontSize"), "40px");

    boxStyle->Merge(buttonStyle);
    CHECK_EQ(boxStyle->GetProperty("fontSize"), "20px");
}
