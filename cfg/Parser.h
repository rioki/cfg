
#ifndef _CFG_PARSER_H_
#define _CFG_PARSER_H_

#include <string>
#include <iostream>

namespace cfg
{
    class Config;

    class Parser
    {
    public:
    
        Parser(Config& config);

        ~Parser();
        
        void parse(std::istream& in);
            
    private:
        enum Token
        {
            NO_TOKEN,
            WHITESPACE,
            OPEN_BRACE,  // [
            CLOSE_BRACE, // ]
            EQUALS,      // =
            IDENTIFIER,  // [a-zA-Z0-9_]+
            STRING,      // "([^"]*)"
            NEWLINE,    // \n|(\r\n)|\r
            FILE_END
        };
        
        Config&       config;
        std::istream* in;

        Token next_token;
        std::string next_value;

        std::string section;

        Token get_next_token(std::string& value);

        Token lex_token(std::string& value);
        Token lex_whitespace(std::string& value);
        Token lex_newline(std::string& value);
        Token lex_identifier(std::string& value);
        Token lex_string(std::string& value);

        void parse_section();

        void parse_section_header();

        void parse_value_pair();


        Parser(const Parser&) = delete;
        const Parser& operator = (const Parser&) = delete;
    };
}

#endif
