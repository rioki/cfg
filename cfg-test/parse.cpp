// 
// cfg - A simple configuration file library.
// 
// Copyright (c) 2015-2017 Sean Farrell <sean.farrell@rioki.org>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 

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
