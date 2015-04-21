#include <iostream>
#include <string>
#include <strstream>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/to_from_ruby.hpp>

#include "edn_parser.h"

//
// based on https://github.com/edn-format/edn
//

%%{
        machine EDN_common;

        cr             = '\n';
        cr_neg         = [^\n];
        ws             = [ ,\t\r\n];
        comment        = ';' cr_neg* cr;
        ignore         = ws | comment;
        begin_keyword  = ':';
        k_nil          = 'nil';
        k_true         = 'true';
        k_false        = 'false';
        begin_value    = [nft\"\-\[\\#] | digit;
        begin_dispatch = '#';
        begin_vector   = '[';
        end_vector     = ']';
        begin_list     = '(';
        end_list       = ')';
        begin_map      = '{';
        end_map        = '{';
        begin_string   = '"';
        begin_number   = digit | '-';

}%%

// ============================================================
// machine for parsing various types
//

%%{
    machine EDN_value;
    include EDN_common;

    write data noerror;

    action parse_nil {
        o = Qnil;
    }
    action parse_false {
        o = Qfalse;
    }
    action parse_true {
        o = Qtrue;
    }

    action parse_string {
        const char *np = EDN_parse_string(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_number {
        const char *np;

        np = EDN_parse_decimal(fpc, pe, o);
        if (np != NULL)
            fexec np;
        else {
            np = EDN_parse_integer(fpc, pe, o);
            if (np != NULL) fexec np;
        }
        fhold; fbreak;
    }

    action parse_vector {
        const char *np = EDN_parse_vector(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action exit { fhold; fbreak; }

    main := (
              k_nil @parse_nil |
              k_false @parse_false |
              k_true @parse_true |
              begin_string >parse_string |
              begin_number >parse_number |
              begin_vector >parse_vector
        ) %*exit;
}%%


const char *edn::Parser::EDN_parse_value(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;

    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_value_first_final) {
        return p;
    }

    return NULL;
}


// ============================================================
// string parsing
//
%%{
    machine EDN_string;
    include EDN_common;

    write data noerror;

    action parse_string {
        if (!EDN_parse_byte_stream(p_save + 1, p, s)) {
            fhold;
            fbreak;
        } else {
            fexec p + 1;
        }
    }

    action exit { fhold; fbreak; }

    main := '"' ((^([\"\\] | 0..0x1f) | '\\'[\"\\/bfnrt] | '\\u'[0-9a-fA-F]{4} | '\\'^([\"\\/bfnrtu]|0..0x1f))* %parse_string) '"' @exit;
}%%


bool edn::Parser::EDN_parse_byte_stream(const char *p, const char *pe, Rice::String& s)
{
    long len = pe - p;

    std::cerr << "+ == " << __FUNCTION__ << " - len: " << len << " == +" << std::endl;

    if (len > 0) {
        std::string buf;
        buf.append(p, len);

        VALUE vs = Rice::protect( rb_str_new2, buf.c_str() );
        VALUE s_utf8 = Rice::protect( rb_enc_associate, vs, rb_utf8_encoding() );
        s = Rice::String(s_utf8);
        return true;
    }

    return false;
}



const char* edn::Parser::EDN_parse_string(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;

    Rice::String s;
    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_string_first_final) {
        o = s;
        return p + 1;
    }
    return NULL;
}

// ============================================================
// decimal parsing grammar
//
%%{
    machine EDN_decimal;
    include EDN_common;

    write data noerror;

    action exit { fhold; fbreak; }

    main := '-'? (
              (('0' | [1-9][0-9]*) '.' [0-9]+ ([Ee] [+\-]?[0-9]+)?)
              | (('0' | [1-9][0-9]*) ([Ee] [+\-]?[0-9]+))
             )  (^[0-9Ee.\-]? @exit );
}%%


const char* edn::Parser::EDN_parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_decimal_first_final) {
        double value;
        o = Parser::buftotype<double>(p_save, p - p_save, value);
        return p + 1;
    }

    return NULL;
}


// ============================================================
// integer parsing grammar
//
%%{
    machine EDN_integer;

    write data noerror;

    action exit { fhold; fbreak; }

    main := '-'? ('0' | [1-9][0-9]*) (^[0-9]? @exit);
}%%

const char* edn::Parser::EDN_parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_integer_first_final) {
        int value;
        o = Parser::buftotype<int>(p_save, p - p_save, value);
        return p + 1;
    }

    return NULL;
}


// ============================================================
// vector parsing machine
//
%%{
    machine EDN_vector;
    include EDN_common;

    write data noerror;

    action parse_value {
        Rice::Object v;
        const char *np = EDN_parse_value(fpc, pe, v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            arr.push(v);
            fexec np;
        }
    }

    action exit { fhold; fbreak; }

    next_element  = ignore* begin_value >parse_value;

    main := begin_vector ignore*
            ((begin_value >parse_value ignore*)
             (ignore* next_element ignore*)*)?
            end_vector @exit;
}%%

//
//
//
const char* edn::Parser::EDN_parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;
    Rice::Array arr;

    %% write init;
    %% write exec;

    if (cs >= EDN_vector_first_final) {
        o = arr;
        return p + 1;
    }

    return NULL;
}


// ============================================================
// main parsing machine
//
%%{
    machine EDN;
    write data nofinal;
    include EDN_common;

    action parse_vector {
        const char* np = EDN_parse_vector(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    #    action parse_hash {
    #        char *np = EDN_parse_hash(fpc, pe, &result);
    #        if (np == NULL) { fhold; fbreak; } else fexec np;
    #    }

    main := ignore* (
                 begin_vector >parse_vector
                 #                 begin_array >parse_array
                 ) ignore*;
}%%

//
//
//
Rice::Object edn::Parser::process(const char* buf, long len)
{
    int cs;
    const char *p;
    const char *pe;
    const char *eof;
    Rice::Object result;

    %% write init;
    p = &buf[0];
    pe = buf + len;
    eof = pe;
    %% write exec;

    if (cs == EDN_error) {
        std::cerr << "Parse error: unexpected value '" << *p << "'" << std::endl;
        return Qnil;
    }

    return result;
}

/*
 * Local variables:
 * mode: c
 * c-file-style: ruby
 * indent-tabs-mode: nil
 * End:
 */
