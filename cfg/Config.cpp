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

#include "Config.h"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <cctype>

#include "Parser.h"

namespace cfg
{
    Config::Config() {}

    Config::~Config() {}
        
    bool Config::has_value(const std::string& section, const std::string& name) const
    {
        auto si = values.find(section);
        if (si != values.end())
        {
            auto vi = si->second.find(name);
            if (vi != si->second.end())
            {
                return true;
            }
        }
        return false;
    }

    const std::string Config::get_value(const std::string& section, const std::string& name) const
    {
        auto si = values.find(section);
        if (si != values.end())
        {
            auto vi = si->second.find(name);
            if (vi != si->second.end())
            {
                return vi->second;
            }
        }
        throw std::logic_error("Value " + section + ":" + name + " does not exist.");
    }

    std::string Config::get_value(const std::string& section, const std::string& name, const std::string& fallback)
    {
        auto si = values.find(section);
        if (si != values.end())
        {
            auto vi = si->second.find(name);
            if (vi != si->second.end())
            {
                return vi->second;
            }
        }

        values[section][name] = fallback;
        return fallback;
    }

    bool isalnum(const std::string& name)
    {
        for (char c : name)
        {
            if (std::isalnum(c) == false && c != '_' && c != '-')
            {
                return false;
            }
        }
        return true;
    }

    void Config::set_value(const std::string& section, const std::string& name, const std::string& value)
    {
        if (isalnum(section) == false)
        {
            throw std::invalid_argument("Section is not an identifier.");
        }
        if (isalnum(name) == false)
        {
            throw std::invalid_argument("Name is not an identifier.");
        }

        values[section][name] = value;
    }

    std::string escape(const std::string value)
    {
        // pure aplha numeric values can be simply written
        if (isalnum(value))
        {
            return value;
        }

        std::stringstream buff;
        buff << '"';

        for (char c : value)
        {
            switch (c)
            {
                case '\'':
                    buff << "\\'";
                    break;
                case '"':
                    buff << "\\\"";
                    break;
                case '\\':
                    buff << "\\\\";
                    break;
                case '\a':
                    buff << "\\a";
                    break;
                case '\b':
                    buff << "\\b";
                    break;
                case '\f':
                    buff << "\\f";
                    break;
                case '\n':
                    buff << "\\n";
                    break;
                case '\r':
                    buff << "\\r";
                    break;
                case '\t':
                    buff << "\\t";
                    break;
                case '\v':
                    buff << "\\v";
                    break;
                default:
                    // TODO escape non printable characters
                    buff << c;
                    break;
            }
        }

        buff << '"';
        return buff.str();
    }

    void Config::save(const std::string& file)
    {
        std::ofstream out(file);

        if (!out.is_open())
        {
            throw std::runtime_error("Failed to open " + file + " for writing.");
        }

        for (auto& section : values) 
        {
            out << "[" << section.first << "]" << std::endl;
            for (auto& value : section.second)
            {
                out << value.first << " = " << escape(value.second) << std::endl;
            }
        }
    }

    void Config::load(const std::string& file)
    {
        std::ifstream in(file);

        if (in.bad())
        {
            throw std::runtime_error("Failed to open " + file + " for reading.");
        }

        Parser parser(*this);
        parser.parse(in, file);
    }
}
