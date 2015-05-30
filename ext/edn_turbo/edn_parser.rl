#include <iostream>
#include <string>

#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/to_from_ruby.hpp>

#include "edn_parser.h"

//
// EDN spec at: https://github.com/edn-format/edn
//
//
// many thanks to Florian Frank for json-ruby which was essential in
// helping me learn about ragel
//

%%{
        machine EDN_common;

        cr             = '\n';
        counter        = ( cr @{ line_number++; } );
        cr_neg         = [^\n];
        ws             = [\t\v\f\r ] | ',' | counter;
        comment        = ';' cr_neg* counter;
        ignore         = ws | comment;

        operators      = [/\.\*!_\?$%&<>\=\-\+];
        symbol_chars   = [a-zA-Z0-9\#:] | operators;

        symbol_first_c = symbol_chars - [0-9\#\:]; # non-numeric, no '#' or ':'

        k_nil          = 'nil';
        k_true         = 'true';
        k_false        = 'false';
        begin_dispatch = '#';
        begin_keyword  = ':';
        begin_char     = '\\';
        begin_value    = alnum | [:\"\{\[\(\\\#] | operators;
# TODO: support - and + symbols. Currently conflicting with numeric values
        begin_symbol   = symbol_first_c - ('-'|'+');
        begin_vector   = '[';
        end_vector     = ']';
        begin_map      = '{';
        begin_list     = '(';
        end_list       = ')';
        string_delim   = '"';
        begin_number   = digit | '-';

        symbol_name    = [\-\+\.]? symbol_first_c (symbol_chars)*;

        symbol         = operators | (symbol_name ('/' symbol_name)?);

        # int / decimal rules
        integer        = '-'? ('0' | [1-9] digit*);
        exp            = ([Ee] [+\-]? digit+);


        action close_err {
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            fexec pe;
        }
}%%

// ============================================================
// machine for parsing various types
//

%%{
    machine EDN_value;
    include EDN_common;

    write data;

    action parse_symbol {
        std::string sym;
        const char *np = parse_symbol(fpc, pe, sym);
        if (np == NULL) { fhold; fbreak; } else {
            if      (sym == "true")  { o = Qtrue; }
            else if (sym == "false") { o = Qfalse; }
            else if (sym == "nil")   { o = Qnil; }
            else {
                o = Parser::make_edn_symbol(sym);
            }
            fexec np;
        }
    }

    action parse_keyword {
        const char *np = parse_keyword(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_char {
        std::string s;
        s += *fpc;
        o = Rice::String(s);
    }

    action parse_string {
        const char *np = parse_string(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_number {
        // try to parse a decimal first
        const char *np = parse_decimal(fpc, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(fpc, pe, o);
        }

        if (np) {
            fexec np;
            fhold;
            fbreak;
        }
        else {
            error(__FUNCTION__, *p);
            fexec pe;
        }
    }

    action parse_vector {
        const char *np = parse_vector(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_list {
        const char *np = parse_list(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_map {
        const char *np = parse_map(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_dispatch {
        const char *np = NULL; //parse_discard(fpc, pe);
        if (np) {
                    std::cerr << "--- PARSE DISCARD - NP is set : '" << np << "'" << std::endl;
        } else {
            //        if (np == NULL) {
            // try a set then
            np = parse_set(fpc, pe, o);

            if (np == NULL) {
                // not a discard.. try a tagged
                np = parse_tagged(fpc, pe, o);
            }
        }

        if (np) {
            //        std::cerr << "--- PARSE DISP NP set : '" << np << "'" << std::endl;
            fexec np;

        } else {
            fhold; fbreak;
            fexec pe;
        }
    }

    action exit { fhold; fbreak; }

    main := (
             begin_dispatch >parse_dispatch |
             begin_char (alnum|punct) >parse_char |
             string_delim >parse_string |
             begin_symbol >parse_symbol |
             begin_keyword >parse_keyword |
             begin_number >parse_number |
             begin_vector >parse_vector |
             begin_list >parse_list |
             begin_map >parse_map
            ) %*exit;
}%%


const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
{
    //                std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_value_first_final) {
        return p;
    }
    else if (cs == EDN_value_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_value_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// symbol parsing
//
%%{
    machine EDN_symbol;
    include EDN_common;

    write data;

    action exit { fhold; fbreak; }

    main := (
             symbol
             ) ignore* (^symbol_chars? @exit);
}%%


const char* edn::Parser::parse_symbol(const char *p, const char *pe, std::string& s)
{
    //std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_symbol_first_final) {
        uint32_t len = p - p_save;
        std::string buf;
        buf.append(p_save, len);

        s = buf;
        return p;
    }
    else if (cs == EDN_symbol_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_symbol_en_main) {} // silence ragel warning
    return NULL;
}




// ============================================================
// keyword parsing
//
%%{
    machine EDN_keyword;
    include EDN_common;

    keyword_chars = symbol_chars;
    keyword_start = symbol_first_c | '#'; # keywords can have '#' after ':'

    keyword_name    = keyword_start (keyword_chars)*;

    write data;

    action exit { fhold; fbreak; }

    main := begin_keyword keyword_name (^keyword_chars? @exit);
}%%


const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_keyword_first_final) {
        uint32_t len = p - p_save - 1; // don't include leading ':' because Rice::Symbol will handle it
        std::string buf;
        buf.append(p_save + 1, len);
        o = Rice::Symbol(buf);
        return p;
    }
    else if (cs == EDN_keyword_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_keyword_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// string parsing
//
%%{
    machine EDN_string;
    include EDN_common;

    write data;

    action parse_string {
        if (!parse_byte_stream(p_save + 1, p, s)) {
            fhold;
            fbreak;
        } else {
            fexec p + 1;
        }
    }

    action exit { fhold; fbreak; }

    main := string_delim (
                          (^([\"\\] | 0..0x1f) |
                           '\\'[\"\\/bfnrt] |
                           '\\u'[0-9a-fA-F]{4} |
                           '\\'^([\"\\/bfnrtu]|0..0x1f))* %parse_string)
            string_delim @err(close_err)
            @exit;
}%%


const char* edn::Parser::parse_string(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "string";
    int cs;
    const char *eof = pe;

    Rice::String s;
    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_string_first_final) {
        o = s;
        return p + 1;
    }
    else if (cs == EDN_string_error) {
        return pe;
    }
    else if (cs == EDN_string_en_main) {} // silence ragel warning
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

    main := (
             (integer '.' digit* (exp? [M]?)) |
             (integer exp)
             )
        (^[0-9Ee.+\-M]? @exit );
}%%


const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_decimal_first_final) {
        double value;
        o = Parser::buftotype<double>(p_save, p - p_save, value);
        return p + 1;
    }
    else if (cs == EDN_decimal_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// integer parsing grammar
//
%%{
    machine EDN_integer;
    include EDN_common;

    write data noerror;

    action exit { fhold; fbreak; }

    main := (integer [M|N]?) (^[0-9MN]? @exit);
}%%

const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_integer_first_final) {
        int value;
        o = Parser::buftotype<int>(p_save, p - p_save, value);
        return p + 1;
    }
    else if (cs == EDN_integer_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// vector parsing machine. EDN_vector_common is used to parse EDN
// vectors and lists since they're both represented as vectors in ruby
//
%%{
    machine EDN_vector_common;
    include EDN_common;

    action parse_value {
        //std::cerr << "--- VECTOR PARSE VALUE: fpc is '" << fpc << "'" << std::endl;
        Rice::Object v;
        const char *np = parse_value(fpc, pe, v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            arr.push(v);
            fexec np;
        }
    }

    action exit { fhold; fbreak; }

    element       = begin_value >parse_value;
    next_element  = ignore* element;
}%%

//
// vector-specific machine
%%{
    machine EDN_vector;
    include EDN_vector_common;

    write data;

    main := begin_vector ignore* (
                                  (element ignore*)
                                  (next_element ignore*)*
                                  )?
            end_vector @err(close_err)
            @exit;
}%%


//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    //std::cerr << __FUNCTION__ << "  -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "vector";

    int cs;
    Rice::Array arr;

    %% write init;
    %% write exec;

    if (cs >= EDN_vector_first_final) {
        o = arr;
        return p + 1;
    }
    else if (cs == EDN_vector_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_vector_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// list parsing machine
//
%%{
    machine EDN_list;
    include EDN_vector_common;

    write data;

    main := begin_list ignore* (
                                (element ignore*)
                                (next_element ignore*)*
                                )?
            end_list @err(close_err)
            @exit;
}%%

//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "list";

    int cs;
    Rice::Array arr;

    %% write init;
    %% write exec;

    if (cs >= EDN_list_first_final) {
        o = arr;
        return p + 1;
    }
    else if (cs == EDN_list_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_list_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// set parsing machine
//
%%{
    machine EDN_set;
    include EDN_common;

    write data noerror;

    begin_set    = '#{';
    end_set      = '}';

    action parse_value {
        Rice::Object set_v;
        const char *np = parse_value(fpc, pe, set_v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            set.push(set_v);
            fexec np;
        }
    }

    action exit { fhold; fbreak; }

    element       = begin_value >parse_value;
    next_element  = ignore* element;

    main := begin_set ignore* (
                               (element ignore*)
                               (next_element ignore*)*
                               )?
            end_set @err(close_err) @exit;
}%%

//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, Rice::Object& o)
{
    //std::cerr << __FUNCTION__ << "     -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "set";

    int cs;
    Rice::Array set; // store as a vector; then convert to a set once done

    %% write init;
    %% write exec;

    if (cs >= EDN_set_first_final) {
        o = make_ruby_set(set);
        return p + 1;
    }
    else if (cs == EDN_set_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// hash parsing
//
%%{
    machine EDN_map;
    include EDN_common;

    end_map        = '}';

    write data;

    action parse_key {
        //        std::cerr << "--- MAP PARSE KEY: fpc is '" << fpc << "'" << std::endl;

        const char *np = parse_value(fpc, pe, k);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            fexec np;
        }
    }

    action parse_value {
        //                std::cerr << "--- MAP PARSE VALUE: fpc is '" << fpc << "'" << std::endl;
        const char *np = parse_value(fpc, pe, v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            map[k] = v;
            fexec np;
        }
    }

    # action to report missing value in k/v pair
    action pair_err {
        error(__FUNCTION__, "map pair not found");
        fexec pe;
    }

    action exit { fhold; fbreak; }

    pair        = ignore* begin_value >parse_key ignore* begin_value >parse_value @err(pair_err);
    next_pair   = ignore* pair;

    main := (
             begin_map
             (pair (next_pair)*)? ignore*
             end_map @err(close_err)
             ) @exit;
}%%


const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "map";

    int cs;
    Rice::Hash map;
    Rice::Object k, v;

    %% write init;
    %% write exec;

    if (cs >= EDN_map_first_final) {
        o = map;
        return p + 1;
    }
    else if (cs == EDN_map_error) {
        return pe;
    }
    else if (cs == EDN_map_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// tagged element parsing - #uuid, #inst, #{, #_
//
%%{
    machine EDN_tagged;
    include EDN_common;

#    inst = (string_delim [0-9\-\+:\.TZ]* string_delim);
#    uuid = (string_delim [a-f0-9\-]* string_delim);

    # tags
    tagged_symbol = alpha [a-zA-z0-9]*;
    built_in_tag  = '#' tagged_symbol;
    user_tag      = '#' tagged_symbol '/' tagged_symbol;

    write data;

    action parse_symbol {
        const char *np = parse_symbol(fpc, pe, sym_name);
        if (np == NULL) { fhold; fbreak; } else { fexec np; }
    }
    action parse_value {
        const char *np = parse_value(fpc, pe, object);
        if (np == NULL) { fhold; fbreak; } else { fexec np; }
    }

    action exit { fhold; fbreak; }

    main := ('#' symbol >parse_symbol ignore* begin_value >parse_value) @exit;
}%%


const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o)
{
    std::string sym_name;
    Rice::Object object;

    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_tagged_first_final) {
        //        std::cerr << __FUNCTION__ << " parse symbol name as '" << sym_name << "', value is: " << object << std::endl;
        o = Parser::tagged_element(sym_name, object);
        return p + 1;
    }
    else if (cs == EDN_tagged_error) {
        return pe;
    }
    else if (cs == EDN_tagged_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// discard
//
%%{
    machine EDN_discard;
    include EDN_common;

    begin_discard = '#_';

    write data noerror;

    action consume_value {
        std::cerr << "--- DISCARD PARSE VALUE: fpc is '" << fpc << "'" << std::endl;
        Rice::Object dummy;
        const char* np = parse_value(fpc, pe, dummy);
        if (np == NULL) { fhold; fbreak; } else {
            //            fexec np;
            np = parse_value(np, pe, dummy);
            fexec np;
        }
    }

    action exit { fhold; fbreak; }

    main := (
             begin_discard ignore* begin_value >consume_value
             ) ignore*
        @exit;
}%%


const char* edn::Parser::parse_discard(const char *p, const char *pe)
{
        std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_discard_first_final) {
        return p + 1;
    }
    //    else if (cs == EDN_discard_error) {
    //        error(__FUNCTION__, *p);
    //        return pe;
    //    }
    else if (cs == EDN_discard_en_main) {} // silence ragel warning

    return NULL;
}


// ============================================================
// main parsing machine
//
%%{
    machine EDN;
    include EDN_common;

    write data nofinal;

    action parse_value {
        const char* np = parse_value(fpc, pe, result);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    main := ignore* (
                     begin_value >parse_value
                     ) ignore*;
}%%

//
//
//
Rice::Object edn::Parser::parse(const char* buf, std::size_t len)
{
    int cs;
    const char *p;
    const char *pe;
    Rice::Object result;

    line_number = 1;

    %% write init;
    p = &buf[0];
    pe = p + len;
    eof = pe; // eof defined in Parser class
    %% write exec;

    if (cs == EDN_error) {
        error(__FUNCTION__, *p);
        return Qnil;
    }
    else if (cs == EDN_en_main) {} // silence ragel warning
    return result;
}

/*
 * Local variables:
 * mode: c
 * c-file-style: ruby
 * indent-tabs-mode: nil
 * End:
 */
