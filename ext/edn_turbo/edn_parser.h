#ifndef EDN_RUBY_EXT_PARSER_H
#define EDN_RUBY_EXT_PARSER_H

#include <string>
#include <sstream>
#include <rice/Object.hpp>
#include <rice/to_from_ruby.hpp>

typedef unsigned char ui8;

namespace edn
{
    //
    // C-extension EDN Parser class representation
    class Parser
    {
    private:
        std::size_t line_number;
        const char* p_save;
        const char* eof;

        Rice::Object parse(const char* s, std::size_t len);

        const char* EDN_parse_decimal(const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_integer(const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_keyword(const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_tagged (const char *p, const char *pe, Rice::Object& o, bool& dicard);
        const char* EDN_parse_string (const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_value  (const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_vector (const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_map    (const char *p, const char *pe, Rice::Object& o);
        const char* EDN_parse_list   (const char *p, const char *pe, Rice::Object& o);

        bool EDN_parse_byte_stream   (const char *p, const char *pe, Rice::String& s);

        void error(const std::string& err, char c) const;
        void error(char err_c) const { error("", err_c); }
        void error(const std::string& err_msg) const { error(err_msg, '\0'); }

        // utility method to convert a primitive in string form to a
        // ruby type
        template <class T>
        static Rice::Object buftotype(const char* p, long len, T& val) {
            std::string buf;
            buf.append(p, len);
            std::istringstream(buf) >> val;
            return to_ruby<T>(val);
        }

    public:
        Parser() : line_number(1), p_save(NULL), eof(NULL) { }

        Rice::Object process(const std::string& data) { return parse(data.c_str(), data.length()); }

        // handle file read from the c-side
        Rice::Object open(const std::string& file);

    }; // Engine

} // namespace

#endif
