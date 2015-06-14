#include <iostream>
#include <string>
#include <limits>
#include <exception>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

#include "edn_parser.h"

namespace edn
{
    //
    // used to determine max number of chars in string value of a type
    template <typename T>
    static std::size_t get_max_chars(T)
    {
        std::stringstream s;
        s << std::fixed << std::numeric_limits<T>::max();
        return s.str().length();
    }

    static const std::size_t LL_max_chars = get_max_chars<>((long) 1);
    static const std::size_t LD_max_chars = get_max_chars<>((double) 1);


    // =================================================================
    //
    //
    VALUE Parser::next()
    {
        VALUE token = EDNT_EOF;

        while (!is_eof())
        {
            bool is_meta = false;
            VALUE v = parse_next(is_meta);

            // check if we've read a discard or metadata token which
            // we must ignore
            if (discard.empty() && !is_meta)
            {
                // valid token
                token = v;
                break;
            }
        }

        return token;
    }

    // reset parsing state
    //
    void Parser::reset_state()
    {
        line_number = 1;
        discard.clear();
        metadata.clear();
    }

    //
    // set a new source
    void Parser::set_source(const char* src, std::size_t len)
    {
        reset_state();
        // set ragel state
        p = src;
        pe = src + len;
        eof = pe;
    }


    //
    // returns an array of metadata value(s) saved in reverse order
    // (right to left) - the ruby side will interpret this
    VALUE Parser::meta() const
    {
        VALUE m = rb_ary_new();

        if (!metadata.empty()) {
            for (std::vector<VALUE>::const_reverse_iterator ii = metadata.rbegin();
                 ii != metadata.rend();
                 ii++) {
                rb_ary_push(m, *ii);
            }
        }

        return m;
    }


    // =================================================================
    // work-around for idiotic rb_protect convention in order to avoid
    // using ruby/rice
    //
    typedef VALUE (edn_rb_f_type)( VALUE arg );

    // we're using at most 2 args
    struct prot_args {
        prot_args(ID m, VALUE arg) :
            method(m), count(1) {
            args[0] = arg;
        }
        prot_args(ID m, VALUE arg1, VALUE arg2) :
            method(m), count(2) {
            args[0] = arg1;
            args[1] = arg2;
        }

        ID method;
        VALUE count;
        VALUE args[2];
    };

    // this allows us to wrap with rb_protect()
    static inline VALUE edn_wrap_funcall2( VALUE arg )
    {
        prot_args* a = reinterpret_cast<prot_args*>(arg);
        return rb_funcall2( edn::rb_mEDNT, a->method, a->count, a->args );
    }

    static inline VALUE edn_prot_rb_funcall( edn_rb_f_type func, VALUE args )
    {
        int error;
        VALUE s = rb_protect( func, args, &error );
        if (error) Parser::throw_error(error);
        return s;
    }

    static inline VALUE edn_prot_rb_new_str(const char* str) {
        int error;
        VALUE s = rb_protect( reinterpret_cast<VALUE (*)(VALUE)>(rb_str_new_cstr),
                              reinterpret_cast<VALUE>(str), &error );
        if (error) Parser::throw_error(error);
        return s;
    }

    static inline VALUE edn_rb_enc_associate_utf8(VALUE str)
    {
        return rb_enc_associate(str, rb_utf8_encoding() );
    }

    // =================================================================
    // utils

    //
    // convert to int.. if string rep has more digits than long can
    // hold, call into ruby to get a big num
    VALUE Parser::integer_to_ruby(const char* str, std::size_t len)
    {
        if (len < LL_max_chars)
        {
            return LONG2NUM(buftotype<long>(str, len));
        }

        // value is outside of range of long type. Use ruby to convert it
        VALUE rb_s = edn_prot_rb_new_str( str );
        prot_args args(EDNT_STR_INT_TO_BIGNUM, rb_s);
        return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
    }

    //
    // as above.. TODO: check exponential..
    VALUE Parser::float_to_ruby(const char* str, std::size_t len)
    {
        if (len < LD_max_chars)
        {
            return rb_float_new(buftotype<double>(str, len));
        }

        // value is outside of range of long type. Use ruby to convert it
        prot_args args(EDNT_STR_DBL_TO_BIGNUM, edn_prot_rb_new_str(str));
        return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
    }


    //
    // copies the string data, unescaping any present values that need to be replaced
    //
    bool Parser::parse_byte_stream(const char *p_start, const char *p_end, VALUE& v_utf8,
                                   bool encode)
    {
        if (p_end > p_start) {
            std::string buf;

            if (encode) {
                if (!to_utf8(p_start, p_end - p_start, buf))
                    return false;
            }
            else {
                buf.append(p_start, p_end - p_start);
            }

            // utf-8 encode
            VALUE vs = edn_prot_rb_new_str(buf.c_str());
            int error;
            v_utf8 = rb_protect( edn_rb_enc_associate_utf8, vs, &error);
            if (error) Parser::throw_error(error);
            return true;
        } else if (p_end == p_start) {
            v_utf8 = rb_str_new("", 0);
            return true;
        }

        return false;
    }

    //
    // handles things like \c, \newline
    //
    bool Parser::parse_escaped_char(const char *p, const char *pe, VALUE& v)
    {
        std::string buf;
        std::size_t len = pe - p;
        buf.append(p, len);

        if (len > 1) {
            if      (buf == "newline") buf = '\n';
            else if (buf == "tab") buf = '\t';
            else if (buf == "return") buf = '\r';
            else if (buf == "space") buf = ' ';
            else if (buf == "formfeed") buf = '\f';
            else if (buf == "backspace") buf = '\b';
            // TODO: is this supported?
            else if (buf == "verticaltab") buf = '\v';
            else return false;
        }

        v = edn_prot_rb_new_str( buf.c_str() );
        return true;
    }


    //
    // get a set representation from the ruby side. See edn_turbo.rb
    VALUE Parser::make_edn_symbol(VALUE sym)
    {
        prot_args args(edn::EDNT_MAKE_EDN_SYMBOL, sym);
        return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
    }

    //
    // get a set representation from the ruby side. See edn_turbo.rb
    VALUE Parser::make_ruby_set(VALUE elems)
    {
        prot_args args(edn::EDNT_MAKE_SET_METHOD, elems);
        return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
    }

    //
    // get an object representation from the ruby side using the given symbol name
    VALUE Parser::tagged_element(VALUE name, VALUE data)
    {
        prot_args args(edn::EDNT_TAGGED_ELEM, name, data);
        return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
    }


    //
    // error reporting
    void Parser::throw_error(int error)
    {
        if (error == 0)
            return;

        VALUE err = rb_errinfo();
        VALUE klass = rb_class_path(CLASS_OF(err));
        VALUE message = rb_obj_as_string(err);
        std::stringstream msg;
        msg << RSTRING_PTR(klass) << " exception: " << RSTRING_PTR(message);
        throw std::runtime_error(msg.str());
    }

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
