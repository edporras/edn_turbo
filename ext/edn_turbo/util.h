#pragma once

namespace edn
{
    extern VALUE rb_mEDN;
    extern VALUE rb_mEDNT;

    extern VALUE EDN_MAKE_SYMBOL_METHOD;
    extern VALUE EDN_MAKE_LIST_METHOD;
    extern VALUE EDN_MAKE_SET_METHOD;
    extern VALUE EDN_MAKE_BIG_DECIMAL_METHOD;
    extern VALUE EDN_TAGGED_ELEM_METHOD;
    extern VALUE EDN_EOF_CONST;

    extern VALUE EDNT_EXTENDED_VALUE_METHOD;

    extern VALUE RUBY_STRING_TO_I_METHOD;
    extern VALUE RUBY_STRING_TO_F_METHOD;
    extern VALUE RUBY_READ_METHOD;

    namespace util
    {
        // defined in edn_parser_util.cc
        VALUE integer_to_ruby(const char* str, std::size_t len);
        VALUE float_to_ruby  (const char* str, std::size_t len);

        VALUE ruby_io_read(VALUE io);

        bool parse_byte_stream (const char *p, const char *pe, VALUE& rslt, bool encode);
        bool parse_escaped_char(const char *p, const char *pe, VALUE& rslt);

        VALUE call_module_fn(VALUE module, ID method);
        VALUE call_module_fn(VALUE module, ID method, VALUE value);
        VALUE call_module_fn(VALUE module, ID method, VALUE value1, VALUE value2);

        // edn_parser_util_unicode.cc
        bool to_utf8(const char *s, uint32_t len, std::string& rslt);
    }
}
