
# Configuration

A simple configuration file library.

## Usage

To read values load and use the get_value API.

    cfg::Config config;    
    config.load("user.cfg");
    
    unsigned int width = config.get_value<unsigned int>("graphic", width, 800);
    
To save values use the set_value and save API.
    
    cfg::Config config;        
    
    config.set_value("graphic", width, 800);
    
    config.save();
    
## API

The library is accessed through the cfg::Config class. It has the following API:

### Load a Configuration File

    void load(const std::string& file)
    
The function `load` loads a configuration file. It a parsing error occurs,
an `std::runtime_error` is raised with the location and cause of the error.

### Save s Configuration File

    void save(std::string file)

The function `save` writes a configuration file. If the function encounters a 
write error, the exception `std::runtime_error` is raised.

### Check Presence of a Value

    bool has_value(const std::string& section, const std::string& name);
    
The function `has_value` checks the presense of a value.

### Read a Value

    const std::string get_value(const std::string& section, const std::string& name) const;

    template <typename Type>
    Type get_value(const std::string& section, const std::string& name) const;

    std::string get_value(const std::string& section, const std::string& name, const std::string& fallback);

    template <typename Type>
    Type get_value(const std::string& section, const std::string& name, const Type& fallback);
    
The function `get_value` read a value from the configuration. The functions
can either have a fallback value set or raise a `std::logic_error` if the
value was not found.

For convenience a type template parameter can be set that converts the internal
string to the desired type. 
*Note:* The type mus be standard streamable.

### Wrtie a Value

    void set_value(const std::string& section, const std::string& name, const std::string& value);

    template <typename Type>
    void set_value(const std::string& section, const std::string& name, const Type& value);

The function `set_value` sets a given value. If the value does not yet exist,
it is created.

For convenience a type template parameter can be set that converts the given 
value to the internal text representation. 
*Note:* The type mus be standard streamable.
    
## Format

The configuration file format loosely follows the INI conversion, but the 
format is stricter.

An example format is as follows:

    # This is a comment
    
    [SectionOne]
    
    valueOne = "This is a string."
    value_two = 123.5
    
    [SectionTwo]

    boolValue = true
    
A few significant different to INI are:

* No values outside of a section.
* All values must be either a identifier, number or string.
* String must be contained within one line.
    
## Formal Syntax    
    
The formal syntax is as follows:

    file := preamble* section*;
    
    preamble := blank | comment;
    
    blank := ws* nl;
    
    comment := ws* "#" [^\r\n]* nl;
    
    section := shead lines;
    
    shead := ws* "[" ws* identifier ws* "]" ws* nl
    
    lines := blank | comment | key_value;
    
    key_value := ws* identifier ws* "=" ws* value ws* nl;

    value: identifier | number | string;
    
    identifier := [a-zA-Z_-][0-9a-zA-Z_-]*;
    
    number := (+|-)?[0-9]+(\.[0-9]+)?
    
    string := \"[^"\r\n]*\"
    
    ws := " " | "\t";
    
    nl := "\n" | "\r" | "\r\n";