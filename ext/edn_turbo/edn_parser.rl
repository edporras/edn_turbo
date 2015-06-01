#include <iostream>
#include <string>
#include <stack>

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

        operators      = [/\.\*!_\?$%&<>\=+\-];
        symbol_start   = alpha;
        symbol_chars   = symbol_start | digit | [\#:_\-\.];

#        k_nil          = 'nil';
#        k_true         = 'true';
#        k_false        = 'false';
        begin_dispatch = '#';
        begin_keyword  = ':';
        begin_char     = '\\';
        begin_value    = alnum | [:\"\{\[\(\\\#] | operators;
        begin_symbol   = symbol_start;
        begin_vector   = '[';
        begin_map      = '{';
        begin_list     = '(';
        string_delim   = '"';
        begin_number   = digit;

        symbol_name    = symbol_start (symbol_chars)*;
        symbol         = (symbol_name ('/' symbol_name)?);

        # int / decimal rules
        integer        = ('0' | [1-9] digit*);
        exp            = ([Ee] [+\-]? digit+);


        action close_err {
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            fhold; fbreak;
        }
}%%

// ============================================================
// machine for parsing various types
//

%%{
    machine EDN_value;
    include EDN_common;

    write data;

    action parse_dispatch {
        const char *np = parse_dispatch(fpc + 1, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_char {
        const char *np = parse_esc_char(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_string {
        const char *np = parse_string(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_keyword {
        const char *np = parse_keyword(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_operator {
        const char *np = parse_operator(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

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

    action exit { fhold; fbreak; }

    main := (
             begin_dispatch >parse_dispatch |
             begin_char >parse_char |
             string_delim >parse_string |
             begin_keyword >parse_keyword |
             operators >parse_operator |
             begin_symbol >parse_symbol |
             begin_number >parse_number |
             begin_vector >parse_vector |
             begin_list >parse_list |
             begin_map >parse_map
            ) %*exit;
}%%


const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
{
    //std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
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
// operator parsing
//
%%{
    machine EDN_operator;
    include EDN_common;

    write data;

    action parse_symbol {
        //        std::cerr << "PARSE OP::SYM '" << (fpc - 1) << "'" << std::endl;
        std::string sym;
        const char *np = parse_symbol(fpc - 1, pe, sym);
        if (np == NULL) { fhold; fbreak; } else {
            o = Parser::make_edn_symbol(sym);
            fexec np;
        }
    }

    action parse_number {
        //std::cerr << "PARSE OP::NUM '" << (fpc - 1) << "'" << std::endl;
        // try to parse a decimal first
        const char *np = parse_decimal(fpc - 1, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(fpc - 1, pe, o);
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

    action parse_operator {
        //        std::cerr << "PARSE OP::OP '" << *(fpc-1) << "', pe: '" << *pe << "'" << std::endl;
        std::string sym;
        sym += *(fpc - 1);
        o = Parser::make_edn_symbol(sym);
    }

    action exit { fhold; fbreak; }

    main := (
             ('-'|'+'|'.') alpha >parse_symbol |
             ('-'|'+') begin_number >parse_number |
             operators ignore* >parse_operator
             ) ^(operators|alpha|digit)? @exit;
}%%


const char* edn::Parser::parse_operator(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;
    std::string op;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_operator_first_final) {
        return p;
    }
    else if (cs == EDN_operator_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_operator_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// escaped char parsing
//
%%{
    machine EDN_escaped_char;
    include EDN_common;

    write data;

    valid_chars = alpha;

    action exit { fhold; fbreak; }

    main := (
             begin_char valid_chars+ ignore*
             ) (^(valid_chars | '\\')? @exit);
}%%


const char* edn::Parser::parse_esc_char(const char *p, const char *pe, Rice::Object& o)
{
    //std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_escaped_char_first_final) {
        if (!Parser::parse_escaped_char(p_save + 1, p, o)) {
            return pe;
        }
        return p;
    }
    else if (cs == EDN_escaped_char_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_escaped_char_en_main) {} // silence ragel warning
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
             operators? symbol |
             operators
             ) ignore* (^(symbol_chars | operators)? @exit);
}%%


const char* edn::Parser::parse_symbol(const char *p, const char *pe, std::string& s)
{
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
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

    keyword_chars = symbol_chars | operators;
    keyword_start = symbol_start | [\#\./];

    keyword_name    = keyword_start (keyword_chars)*;

    write data;

    action exit { fhold; fbreak; }

    main := begin_keyword keyword_name (^keyword_chars? @exit);
}%%


const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;

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
        if (!Parser::parse_byte_stream(p_save + 1, p, s)) {
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
                           '\\'^([\"\\/bfnrtu]|0..0x1f))* %parse_string
                          ) :>> string_delim @err(close_err) @exit;
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

    main := ('-'|'+')? (
                        (integer '.' digit* (exp? [M]?)) |
                        (integer exp)
                        ) (^[0-9Ee.+\-M]? @exit );
}%%


const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_decimal_first_final) {
        o = Parser::float_to_ruby(p_save, p - p_save);
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

    main := (
             ('-'|'+')? (integer [MN]?)
             ) (^[0-9MN+\-]? @exit);
}%%

const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;

    %% write init;
    p_save = p;
    %% write exec;

    if (cs >= EDN_integer_first_final) {
        o = Parser::integer_to_ruby(p_save, p - p_save);
        return p + 1;
    }
    else if (cs == EDN_integer_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// EDN_sequence_common is used to parse EDN containers - elements are
// initially stored in a rice array and then the final corresponding
// container is built from the list (although, for vectors, lists, and
// sets the same array is used)
//
%%{
    machine EDN_sequence_common;
    include EDN_common;

    action parse_value {
        Rice::Object v;
        const char *np = parse_value(fpc, pe, v);
        if (np == NULL) {
            fhold; fbreak;
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept
            if (!discard.empty()) {
                discard.pop();
            }
            else {
                // otherwise we add it to the sequence
                arr.push(v);
            }
            fexec np;
        }
    }

    element       = begin_value >parse_value;
    next_element  = ignore* element;
    sequence      = ((element ignore*) (next_element ignore*)*);

    action exit { fhold; fbreak; }
}%%

//
// vector-specific machine
%%{
    machine EDN_vector;
    include EDN_sequence_common;

    end_vector     = ']';

    write data;

    main := begin_vector (
                          ignore* sequence? :>> end_vector
                          )
                          @err(close_err) @exit;
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
    include EDN_sequence_common;

    end_list       = ')';

    write data;

    main := begin_list (
                        ignore* sequence? :>> end_list
                        ) @err(close_err) @exit;
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
    include EDN_sequence_common;

    write data;

    begin_set    = '{';
    end_set      = '}';

    main := begin_set (
                       ignore* sequence? :>> end_set
                       ) @err(close_err) @exit;
}%%

//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "     -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "set";

    int cs;
    Rice::Array arr; // store as a vector; then convert to a set once done

    %% write init;
    %% write exec;

    if (cs >= EDN_set_first_final) {
        o = Parser::make_ruby_set(arr);
        return p + 1;
    }
    else if (cs == EDN_set_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_set_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// hash parsing
//
%%{
    machine EDN_map;
    include EDN_sequence_common;

    end_map        = '}';

    write data;

    # action to report missing value in k/v pair
    action pair_err {
        error(__FUNCTION__, "map pair not found");
        fexec pe;
    }

    main := begin_map (
                       ignore* (sequence)? :>> end_map
                       ) @err(close_err) @exit;
}%%


const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "map";
    Rice::Array arr;
    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_map_first_final) {

        if ((arr.size() % 2) != 0) {
            error(__FUNCTION__, "odd number of elements in map");
            return pe;
        }

        Rice::Hash map;
        while (arr.size())
        {
            Rice::Object k = arr.shift();
            map[k] = arr.shift();
        }

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

#    inst = (string_delim [0-9+\-:\.TZ]* string_delim);
#    uuid = (string_delim [a-f0-9\-]* string_delim);

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

    main := (symbol >parse_symbol ignore* begin_value >parse_value) @exit;
}%%


const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o)
{
    //        std::cerr << __FUNCTION__ << " p '" << p << "'" << std::endl;
    std::string sym_name;
    Rice::Object object;

    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_tagged_first_final) {
        //std::cerr << __FUNCTION__ << " parse symbol name as '" << sym_name << "', value is: " << object << std::endl;
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

    write data;

    begin_discard = '_';

    action discard_value {
        const char *np = parse_value(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else { discard.push(o); fexec np; }
    }

    action exit {
        fhold; fbreak;
    }

    main := begin_discard ignore* (
                                   begin_value >discard_value
                                   ) @exit;
}%%


const char* edn::Parser::parse_discard(const char *p, const char *pe)
{
    //std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    int cs;
    Rice::Object o;

    %% write init;
    %% write exec;

    if (cs >= EDN_discard_first_final) {
        return p + 1;
    }
    else if (cs == EDN_discard_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_discard_en_main) {} // silence ragel warning

    return NULL;
}



// ============================================================
// dispatch
//
%%{
    machine EDN_dispatch;
    include EDN_common;

    write data;

    action parse_discard {
        const char *np = parse_discard(fpc, pe);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_set {
        const char *np = parse_set(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action parse_tagged {
        const char *np = parse_tagged(fpc, pe, o);
        if (np == NULL) { fhold; fbreak; } else fexec np;
    }

    action exit { fhold; fbreak; }

    main := (
             ('_' >parse_discard) |
             '{' >parse_set |
             alpha >parse_tagged
             ) @exit;
}%%


const char* edn::Parser::parse_dispatch(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    int cs;

    %% write init;
    %% write exec;

    if (cs >= EDN_dispatch_first_final) {
        return p + 1;
    }
    else if (cs == EDN_dispatch_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_dispatch_en_main) {} // silence ragel warning

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
        if (np == NULL) { fexec pe; fbreak; } else fexec np;
    }

    element       = begin_value >parse_value;
    next_element  = ignore* element;
    sequence      = ((element ignore*) (next_element ignore*)*);

    # TODO: check this. Using a sequence to handle cases with a discard
    main := ignore* sequence? ignore*;
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

    // init
    line_number = 1;
    p_save = NULL;
    while (!discard.empty())
        discard.pop();

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
