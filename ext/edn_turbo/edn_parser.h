#ifndef EDN_RAGEL_PARSER_H
#define EDN_RAGEL_PARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <stack>

#include <ruby/ruby.h>


namespace edn
{
    extern VALUE rb_mEDNT;
    extern VALUE EDNT_MAKE_SYMBOL_METHOD;
    extern VALUE EDNT_MAKE_LIST_METHOD;
    extern VALUE EDNT_MAKE_SET_METHOD;
    extern VALUE EDNT_TAGGED_ELEM_METHOD;
    extern VALUE EDNT_EXTENDED_VALUE_METHOD;

    extern VALUE EDNT_STRING_TO_I_METHOD;
    extern VALUE EDNT_STRING_TO_F_METHOD;

    extern VALUE EDNT_EOF_CONST;


    //
    // C-extension EDN Parser class representation
    class Parser
    {
    public:
        Parser() : p(NULL), pe(NULL), eof(NULL), line_number(1) {
            new_meta_list();
        }
        ~Parser() { reset_state(); del_top_meta_list(); }

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
        std::stack<std::vector<VALUE>* > metadata;

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

        static VALUE make_edn_type(ID method, VALUE value);
        static VALUE make_edn_type(ID method, VALUE value1, VALUE value2);

        // metadata
        VALUE ruby_meta();
        void  new_meta_list() { metadata.push( new std::vector<VALUE>() ); }
        void  del_top_meta_list() { delete metadata.top(); metadata.pop(); }
        void  append_to_meta(VALUE m) { metadata.top()->push_back(m); }
        bool  meta_empty() const { return metadata.top()->empty(); }
        std::size_t meta_size() const { return metadata.top()->size(); }

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
