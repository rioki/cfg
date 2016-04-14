
#include <cfg/Config.h>
#include "rtest.h"

SUITE(parse)
{
    TEST(simple)
    {
        cfg::Config config;
        config.load("simple.cfg");

        CHECK(config.has_value("General", "greeting"));
        CHECK_EQUAL("Hello config!", config.get_value("General", "greeting"));
    }

    TEST(types)
    {
        cfg::Config config;
        config.load("types.cfg");

        CHECK_EQUAL("true",  config.get_value("Types", "bool0"));
        CHECK_EQUAL("false", config.get_value("Types", "bool1"));
        CHECK_EQUAL("TRUE",  config.get_value("Types", "bool2"));
        CHECK_EQUAL("FALSE", config.get_value("Types", "bool3"));	

        CHECK_EQUAL("This is a string.", config.get_value("Types", "string"));

        CHECK_EQUAL("yolo", config.get_value("Types", "identifier"));

        CHECK_EQUAL("123", config.get_value("Types", "integer0"));
        CHECK_EQUAL("+123", config.get_value("Types", "integer1"));
        CHECK_EQUAL("-123", config.get_value("Types", "integer2"));

        CHECK_EQUAL("1.0", config.get_value("Types", "float0"));
        CHECK_EQUAL("+1.0", config.get_value("Types", "float1"));
        CHECK_EQUAL("-1.0", config.get_value("Types", "float2"));			
    }
}
