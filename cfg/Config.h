
#ifndef _CFG_CONFIG_H_
#define _CFG_CONFIG_H_

#include <string>
#include <map>
#include <iostream>
#include <sstream>

namespace cfg
{
    class Config
    {
    public:
        Config();

        ~Config();
        
        bool has_value(const std::string& section, const std::string& name) const;

        const std::string get_value(const std::string& section, const std::string& name) const;

        template <typename Type>
        Type get_value(const std::string& section, const std::string& name) const;

        std::string get_value(const std::string& section, const std::string& name, const std::string& fallback);

        template <typename Type>
        Type get_value(const std::string& section, const std::string& name, const Type& fallback);

        void set_value(const std::string& section, const std::string& name, const std::string& value);

        template <typename Type>
        void set_value(const std::string& section, const std::string& name, const Type& value);

        void save(const std::string& file);

        void load(const std::string& file);

    private:
        typedef std::map<std::string, std::string> Section;
        std::map<std::string, Section> values;
    };

    template <typename Type>
    Type Config::get_value(const std::string& section, const std::string& name) const
    {
        std::string str = get_value(section, name);

        Type value;

        std::stringstream buff(str);
        buff >> value;

        return value;
    }

    template <typename Type>
    Type Config::get_value(const std::string& section, const std::string& name, const Type& fallback)
    {
        auto si = values.find(section);
        if (si != values.end())
        {
            auto vi = si->second.find(name);
            if (vi != si->second.end())
            {                 
                 Type value;
                 std::stringstream buff(vi->second);
                 buff >> value;
                 return value;
            }
        }

        std::stringstream buff;
        buff << fallback;

        values[section][name] = buff.str();
        return fallback;
    }

    template <typename Type>
    void Config::set_value(const std::string& section, const std::string& name, const Type& value)
    {
        std::stringstream buff;
        buff << value;
        set_value(section, name, value);
    }
}

#endif
