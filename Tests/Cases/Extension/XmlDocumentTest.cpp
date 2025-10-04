#include "DgeX/Extension/XmlDocument.h"

#include "doctest/Common.h"
#include "doctest/doctest.h"

#include <DgeX/DgeX.h>

TEST_CASE("XmlDocument Load Test")
{
    using namespace DgeX;
    using namespace DgeX::Ext;

    SUBCASE("Successful Load")
    {
        const auto FILE_PATH = GetResource("Extension", "XmlDocumentTest.xml");

        XmlDocument doc;
        REQUIRE(doc.LoadFromFile(FILE_PATH.string()));

        XmlElement root = doc.GetRoot();
        REQUIRE(root.IsValid());

        CHECK_STR_EQ(root.Name(), "Page");
        CHECK_STR_EQ(root.Attribute("name"), "main");

        CHECK_EQ(root.FirstChild(), root.FirstChild("Styles"));
        XmlElement styles = root.FirstChild();
        REQUIRE(styles.IsValid());
        REQUIRE(styles.FirstChild().IsValid());
        REQUIRE(styles.LastChild().IsValid());
        CHECK_STR_EQ(styles.FirstChild().Attribute("name"), "box");
        CHECK_STR_EQ(styles.LastChild().Attribute("name"), "button");

        XmlElement box = root.LastChild();
        REQUIRE(box.IsValid());
        CHECK_EQ(box.AttributeAs<Color>("backgroundColor", Color::Black), Color::Red);

        XmlElement button = box.FirstChild();
        REQUIRE(button.IsValid());
        XmlElement hover = button.FirstChild();
        REQUIRE(hover.IsValid());
        XmlElement property = hover.FirstChild();
        REQUIRE(property.IsValid());
        CHECK_EQ(property.AttributeAs<Color>("value", Color::Black), Color::Green);

        XmlAttribute attribute = property.FirstAttribute();
        REQUIRE(attribute.IsValid());
        CHECK_STR_EQ(attribute.Name(), "name");
        CHECK_STR_EQ(attribute.Value(), "backgroundColor");
        attribute = attribute.Next();
        REQUIRE(attribute.IsValid());
        CHECK_STR_EQ(attribute.Name(), "value");
        CHECK_STR_EQ(attribute.Value(), "#00AA00");
    }

    SUBCASE("Failed Load")
    {
        const auto FILE_PATH = GetResource("Extension", "NotFound.xml");
        XmlDocument doc(FILE_PATH.string());

        CHECK_FALSE(doc.GetRoot().IsValid());
    }
}
