#ifndef EDN_TURBO_PARSER_H
#define EDN_TURBO_PARSER_H

#include <string>
#include <sstream>
#include <stack>

// always include rice headers before ruby.h
#include <rice/Object.hpp>
#include <rice/to_from_ruby.hpp>

#include <ruby/ruby.h>


namespace edn
{
    extern VALUE rb_mEDNT;
    extern VALUE EDNT_MAKE_EDN_SYMBOL;
    extern VALUE EDNT_MAKE_SET_METHOD;
    extern VALUE EDNT_TAGGED_ELEM;
    extern VALUE EDNT_STR_INT_TO_BIGNUM;
    extern VALUE EDNT_STR_DBL_TO_BIGNUM;

    //
    // C-extension EDN Parser class representation
    class Parser
    {
    private:

        std::size_t line_number;
        const char* p_save;
        const char* eof;
        std::stack<Rice::Object> discard;

        Rice::Object parse(const char* s, std::size_t len);

        const char* parse_value   (const char *p, const char *pe, Rice::Object& o);
        const char* parse_string  (const char *p, const char *pe, Rice::Object& o);
        const char* parse_keyword (const char *p, const char *pe, Rice::Object& o);
        const char* parse_decimal (const char *p, const char *pe, Rice::Object& o);
        const char* parse_integer (const char *p, const char *pe, Rice::Object& o);
        const char* parse_operator(const char *p, const char *pe, Rice::Object& o);
        const char* parse_esc_char(const char *p, const char *pe, Rice::Object& o);
        const char* parse_symbol  (const char *p, const char *pe, std::string& s);
        const char* parse_vector  (const char *p, const char *pe, Rice::Object& o);
        const char* parse_list    (const char *p, const char *pe, Rice::Object& o);
        const char* parse_map     (const char *p, const char *pe, Rice::Object& o);
        const char* parse_dispatch(const char *p, const char *pe, Rice::Object& o);
        const char* parse_set     (const char *p, const char *pe, Rice::Object& o);
        const char* parse_discard (const char *p, const char *pe);
        const char* parse_tagged  (const char *p, const char *pe, Rice::Object& o);

        // defined in edn_parser_unicode.cc
        static bool to_utf8(const char *s, std::size_t len, std::string& rslt);

        // defined in edn_parser_util.cc
        static VALUE integer_to_ruby(const char* str, std::size_t len);
        static VALUE float_to_ruby  (const char* str, std::size_t len);

        static bool parse_byte_stream (const char *p, const char *pe, VALUE& rslt, bool encode);
        static bool parse_escaped_char(const char *p, const char *pe, VALUE& rslt);

        static VALUE make_edn_symbol(const std::string& name);
        static VALUE make_ruby_set(const VALUE elems);
        static VALUE tagged_element(const std::string& name, VALUE data);

        void error(const std::string& f, const std::string& err, char c) const;
        void error(const std::string& f, char err_c) const { error(f, "", err_c); }
        void error(const std::string& f, const std::string& err_msg) const { error(f, err_msg, '\0'); }

        // utility method to convert a primitive in string form to a
        // ruby type
        template <class T>
        static Rice::Object buftotype(const char* p, std::size_t len) {
            T val;
            std::string buf;
            buf.append(p, len);
            std::istringstream(buf) >> val;
            return to_ruby<T>(val);
        }

    public:
        Parser() : line_number(1), p_save(NULL), eof(NULL) { }

        Rice::Object process(const std::string& data) { return parse(data.c_str(), data.length()); }

    }; // Engine

} // namespace

#endif
