#ifndef EDN_RAGEL_PARSER_H
#define EDN_RAGEL_PARSER_H

#include <string>
#include <sstream>
#include <vector>

#include <ruby/ruby.h>


namespace edn
{
    extern VALUE rb_mEDNT;
    extern VALUE EDNT_MAKE_EDN_SYMBOL;
    extern VALUE EDNT_MAKE_SET_METHOD;
    extern VALUE EDNT_TAGGED_ELEM;
    extern VALUE EDNT_BIND_META_TO_VALUE;
    extern VALUE EDNT_STR_INT_TO_BIGNUM;
    extern VALUE EDNT_STR_DBL_TO_BIGNUM;
    extern VALUE EDNT_EOF;

    //
    // C-extension EDN Parser class representation
    class Parser
    {
    public:
        Parser() : p(NULL), pe(NULL), eof(NULL), line_number(1) { }

        // change input source
        void set_source(const char* src, std::size_t len);

        bool is_eof() const { return (p == eof); }

        // parses an entire stream
        VALUE parse(const char* s, std::size_t len);

        // returns the next element in the current stream
        VALUE next();

        static void throw_error(int error);

    private:
        // ragel needs these
        const char* p;
        const char* pe;
        const char* eof;
        std::size_t line_number;
        std::vector<VALUE> discard;
        std::vector<VALUE> metadata;

        void reset_state();

        const char* parse_value   (const char *p, const char *pe, VALUE& v);
        const char* parse_string  (const char *p, const char *pe, VALUE& v);
        const char* parse_keyword (const char *p, const char *pe, VALUE& v);
        const char* parse_decimal (const char *p, const char *pe, VALUE& v);
        const char* parse_integer (const char *p, const char *pe, VALUE& v);
        const char* parse_operator(const char *p, const char *pe, VALUE& v);
        const char* parse_esc_char(const char *p, const char *pe, VALUE& v);
        const char* parse_symbol  (const char *p, const char *pe, VALUE& v);
        const char* parse_vector  (const char *p, const char *pe, VALUE& v);
        const char* parse_list    (const char *p, const char *pe, VALUE& v);
        const char* parse_map     (const char *p, const char *pe, VALUE& v);
        const char* parse_dispatch(const char *p, const char *pe, VALUE& v);
        const char* parse_set     (const char *p, const char *pe, VALUE& v);
        const char* parse_discard (const char *p, const char *pe);
        const char* parse_tagged  (const char *p, const char *pe, VALUE& v);
        const char* parse_meta    (const char *p, const char *pe);

        enum eTokenState { TOKEN_OK, TOKEN_ERROR, TOKEN_IS_DISCARD, TOKEN_IS_META };

        eTokenState parse_next(VALUE& value);

        // defined in edn_parser_unicode.cc
        static bool to_utf8(const char *s, std::size_t len, std::string& rslt);

        // defined in edn_parser_util.cc
        static VALUE integer_to_ruby(const char* str, std::size_t len);
        static VALUE float_to_ruby  (const char* str, std::size_t len);

        static bool parse_byte_stream (const char *p, const char *pe, VALUE& rslt, bool encode);
        static bool parse_escaped_char(const char *p, const char *pe, VALUE& rslt);

        static VALUE make_edn_symbol(VALUE sym);
        static VALUE make_ruby_set(VALUE elems);
        static VALUE tagged_element(VALUE name, VALUE data);

        // metadata
        VALUE ruby_meta();
        VALUE bind_meta_to_value(VALUE value);

        // utility method to convert a primitive in string form to a
        // ruby type
        template <class T>
        static inline T buftotype(const char* p, std::size_t len) {
            T val;
            std::string buf;
            buf.append(p, len);
            std::istringstream(buf) >> val;
            return val;
        }

        void error(const std::string& f, const std::string& err, char c) const;
        void error(const std::string& f, char err_c) const { error(f, "", err_c); }
        void error(const std::string& f, const std::string& err_msg) const { error(f, err_msg, '\0'); }
    }; // Engine

} // namespace

#endif
