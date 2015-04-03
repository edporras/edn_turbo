#include <iostream>
#include <string>
#include <strstream>
#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/to_from_ruby.hpp>

#include "edn_parser.h"

//
// based on https://github.com/edn-format/edn
//

%%{
        machine EDN_common;

        ws             = [ ,\t\r\n];
        begin_comment  = ';';
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

%%{
    machine EDN_value;
    include EDN_common;

    write data noerror;

    action parse_nil {
        result = Qnil;
    }
    action parse_false {
        result = Qfalse;
    }
    action parse_true {
        result = Qtrue;
    }

    action parse_number {
        const char *np;

        np = EDN_parse_decimal(fpc, pe, result);
        if (np != NULL) {
            fexec np;
        } else {
            np = EDN_parse_integer(fpc, pe, result);
            if (np != NULL) fexec np;
        }
        fhold; fbreak;
    }

    action parse_vector {
        const char *np = EDN_parse_vector(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action exit { fhold; fbreak; }

    main := (
              k_nil @parse_nil |
              k_false @parse_false |
              k_true @parse_true |
              begin_number >parse_number |
              begin_vector >parse_vector
        ) %*exit;
}%%


const char *edn::Parser::EDN_parse_value(const char *p, const char *pe, Rice::Object& result)
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

    next_element  = ws* begin_value >parse_value;

    main := begin_vector ws*
            ((begin_value >parse_value ws*)
             (ws* next_element ws*)*)?
            end_vector @exit;
}%%

//
//
//
const char* edn::Parser::EDN_parse_vector(const char *p, const char *pe, Rice::Object& result)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;
    Rice::Array arr;

    %% write init;
    %% write exec;

    if (cs >= EDN_vector_first_final) {
        result = arr;
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

    main := ws* (
                 begin_vector >parse_vector
                 #                 begin_array >parse_array
                 ) ws*;
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
