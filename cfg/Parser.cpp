
#include "Parser.h"
#include "Config.h"

#include <stdexcept>

namespace cfg
{
    Parser::Parser(Config& c)
    : config(c), in(nullptr), next_token(NO_TOKEN) {}

    Parser::~Parser() {}
        
    void Parser::parse(std::istream& i)
    {
        in = &i;

        // prep the look ahead
        std::string dummy;
        get_next_token(dummy);

        while (next_token != FILE_END)
        {
            parse_section();
        }
    }

    Parser::Token Parser::get_next_token(std::string& value)
    {
        Token token = next_token;
        value = next_value;

        next_token = lex_token(next_value);
        while (next_token == WHITESPACE)
        {
            next_token = lex_token(next_value);
        }

        return token;
    }
            
    Parser::Token Parser::lex_token(std::string& value)
    {
        int c = in->get();
        switch (c)
        {               
            case ' ': case '\t': case '\v':
                value.push_back(c);
                return lex_whitespace(value);
            case '\n': case '\r':
                value.push_back(c);
                return lex_newline(value);
            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': 
            case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
            case 's': case 't': case 'u': case 'v': case 'w': case 'x':
            case 'y': case 'z': 
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': 
            case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
            case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
            case 'Y': case 'Z':
            case '_': case '-':
                value.push_back(c);
                return lex_identifier(value);
            case '"':
                return lex_string(value);
            case '[':
                value = "[";
                return OPEN_BRACE;
            case ']':
                value = "]";
                return CLOSE_BRACE;
            case '=':
                value = "=";
                return EQUALS;
            case EOF:
                return FILE_END;
            default:
                value.push_back(c);
                throw std::runtime_error("Unexpected " + value + ".");
        }
    }

    Parser::Token Parser::lex_whitespace(std::string& value)
    {
        int c = in->get();
        while (true)        
        {
            switch (c)
            {
                case ' ': case '\t': case '\v':
                    value.push_back(c);
                    break;
                default:
                    in->unget();
                    return WHITESPACE;
            }
            c = in->get();
        }
    }

    Parser::Token Parser::lex_newline(std::string& value)
    {
        int c = in->get();
        switch (c)
        {
            case '\n': case '\r':
                if (c != value[0])
                {
                    // \r\n or \n\r
                    value.push_back(c);
                }
                else
                {
                    // treat \n \n as two newline, obviously
                    in->unget();                            
                }
                return NEWLINE;
            default:
                in->unget();
                return NEWLINE;
        }
    }

    Parser::Token Parser::lex_identifier(std::string& value)
    {
        int c = in->get();
        while (true)        
        {
            switch (c)
            {                            
                case '0': case '1': case '2': case '3': case '4': 
                case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': 
                case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                case 'y': case 'z': 
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': 
                case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                case 'Y': case 'Z': 
                case '_': case '-':
                    value.push_back(c);
                    break;
                default:
                    in->unget();
                    return IDENTIFIER;
            }
            c = in->get();
        }        
    }
    
    Parser::Token Parser::lex_string(std::string& value)
    {
        int c = in->get();
        while (true)        
        {
            switch (c)
            {
                case '"': 
                    return STRING;
                case '\n': case '\r':
                    throw std::runtime_error("Unexpected newline in string.");
                case '\\':
                    c = in->get();
                    switch (c)
                    {
                        case '\'':
                            value.push_back('\'');
                            break;
                        case '"':
                            value.push_back('"');
                            break;
                        case '\\':
                            value.push_back('\\');
                            break;
                        case 'a':
                            value.push_back('\a');
                            break;
                        case 'b':
                            value.push_back('\b');
                            break;
                        case 'f':
                            value.push_back('\f');
                            break;
                        case 'n':
                            value.push_back('\n');
                            break;
                        case 'r':
                            value.push_back('\r');
                            break;
                        case 't':
                            value.push_back('\t');
                            break;
                        case 'v':
                            value.push_back('\v');
                            break;
                        default:
                            throw std::runtime_error("Unknown escape sequence.");
                            break;
                    }
                    break;
                default:
                    value.push_back(c);
                    break;
            }
            c = in->get();
        }
    }

    void Parser::parse_section() 
    {
        parse_section_header();

        while (next_token == IDENTIFIER) 
        {
            parse_value_pair();
        }
    }

    void Parser::parse_section_header() 
    {
        std::string value;
        Token t = get_next_token(value);

        if (t != OPEN_BRACE) 
        {
            throw std::runtime_error("Expected open breace.");
        }

        t = get_next_token(value);
        if (t != IDENTIFIER)
        {
            throw std::runtime_error("Expected identifier.");
        }
        section = value;

        t = get_next_token(value);
        if (t != CLOSE_BRACE)
        {
            throw std::runtime_error("Expected close brace.");
        }

        t = get_next_token(value);
        if (t != NEWLINE && t != FILE_END)
        {
            throw std::runtime_error("Expected newline.");
        }

    }

    void Parser::parse_value_pair() 
    {
        std::string value;
        Token t = get_next_token(value);

        if (t == NEWLINE)
        {
            // accept empty line
            return;
        }

        if (t != IDENTIFIER) 
        {
            throw std::runtime_error("Expected identifier.");
        }
        std::string name = value;

        t = get_next_token(value);
        if (t != EQUALS)
        {
            throw std::runtime_error("Expected equals.");
        }

        if (t != IDENTIFIER && t != STRING) 
        {
            throw std::runtime_error("Expected identifier or string.");
        }
        
        config.set_value(section, name, value);

        t = get_next_token(value);
        if (t != NEWLINE && t != FILE_END)
        {
            throw std::runtime_error("Expected newline.");
        }
    }
}
