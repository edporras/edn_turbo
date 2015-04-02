#ifndef EDN_RUBY_EXT_PARSER_H
#define EDN_RUBY_EXT_PARSER_H

#include <string>
#include <rice/Object.hpp>

typedef unsigned char ui8;

namespace edn
{
    //
    // C-extension EDN Parser class representation
    class Parser
    {
    private:
        const char* p_save;
        std::string buf;

        Rice::Object execute(const char* b, long len);
        const char* EDN_parse_decimal(const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_integer(const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_value(const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_vector(const char *p, const char *pe, Rice::Object& o);

    public:
        Parser() : p_save(NULL) { }

        Rice::Object parse(const std::string& file);

    }; // Engine

} // namespace

#endif
