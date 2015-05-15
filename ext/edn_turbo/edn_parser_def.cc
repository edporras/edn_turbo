#include <iostream>
#include <string>

#include <rice/String.hpp>
#include <rice/Array.hpp>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

#include "edn_parser.h"

namespace edn
{
    //
    // copies the string data, unescaping any present values that need to be replaced
    //
    bool Parser::parse_byte_stream(const char *p_start, const char *p_end, Rice::String& s)
    {
        if (p_end > p_start) {
            std::string buf;
            std::size_t len = p_end - p_start;

            if (unicode_to_utf8(p_start, len, buf))
            {
                // utf-8 encode
                VALUE vs = Rice::protect( rb_str_new2, buf.c_str() );
                VALUE s_utf8 = Rice::protect( rb_enc_associate, vs, rb_utf8_encoding() );
                s = Rice::String(s_utf8);
                return true;
            }
        }

        return false;
    }


    //
    // get the date representation from the ruby side. See edn_turbo.rb
    Rice::Object Parser::make_ruby_date(const std::string& s)
    {
        VALUE rb_s = Rice::protect(rb_str_new2, s.c_str());
        return Rice::protect(rb_funcall, rb_mEDNT, EDNT_RFC3339_DATE_METHOD, 1, rb_s);
    }

    //
    // get a set representation from the ruby side. See edn_turbo.rb
    Rice::Object Parser::make_ruby_set(const Rice::Array& elems)
    {
        return Rice::protect(rb_funcall, rb_mEDNT, EDNT_MAKE_SET_METHOD, 1, elems.value());
    }


    //
    // error reporting
    void Parser::error(const std::string& func, const std::string& err, char c) const
    {
        std::cerr << "Parse error "
            //            "from " << func << "() "
            ;
        if (err.length() > 0)
            std::cerr << "(" << err << ") ";
        if (c != '\0')
            std::cerr << "at '" << c << "' ";
        std::cerr << "on line " << line_number << std::endl;
    }
}
