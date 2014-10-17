
#ifndef _CFG_CONFIG_H_
#define _CFG_CONFIG_H_

#include <string>
#include <map>
#include <iostream>

namespace cfg
{
    class Config
    {
    public:
        Config();

        ~Config();
        
        bool has_value(const std::string& section, const std::string& name) const;

        const std::string get_value(const std::string& section, const std::string& name) const;

        void set_value(const std::string& section, const std::string& name, const std::string& value);

        void save(const std::string& file);

        void load(const std::string& file);

    private:
        typedef std::map<std::string, std::string> Section;
        std::map<std::string, Section> values;
    };
}

#endif
