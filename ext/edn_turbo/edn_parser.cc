
#line 1 "edn_parser.rl"
#include <iostream>
#include <string>
#include <stack>

#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/to_from_ruby.hpp>
#include <rice/Exception.hpp>

#include "edn_parser.h"

//
// EDN spec at: https://github.com/edn-format/edn
//
//
// many thanks to Florian Frank for json-ruby which was essential in
// helping me learn about ragel
//


#line 62 "edn_parser.rl"


// ============================================================
// machine for parsing various EDN token types
//


#line 32 "edn_parser.cc"
static const int EDN_value_start = 1;
static const int EDN_value_first_final = 2;
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 170 "edn_parser.rl"



const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 49 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 178 "edn_parser.rl"

#line 56 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 34: goto tr2;
		case 35: goto tr3;
		case 40: goto tr4;
		case 58: goto tr6;
		case 91: goto tr8;
		case 92: goto tr9;
		case 95: goto tr0;
		case 123: goto tr10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 42 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr0;
		} else if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 47 )
				goto tr0;
		} else
			goto tr0;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr7;
		} else
			goto tr7;
	} else
		goto tr5;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 106 "edn_parser.rl"
	{
        // stand-alone operators *, +, -, etc.
        const char *np = parse_operator(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr2:
#line 74 "edn_parser.rl"
	{
        // string types within double-quotes
        const char *np = parse_string(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr3:
#line 151 "edn_parser.rl"
	{
        // handles tokens w/ leading # ("#_", "#{", and tagged elems)
        const char *np = parse_dispatch(p + 1, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr4:
#line 139 "edn_parser.rl"
	{
        // (
        const char *np = parse_list(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr5:
#line 86 "edn_parser.rl"
	{
        // tokens w/ leading digits: non-negative integers & decimals.
        // try to parse a decimal first
        const char *np = parse_decimal(p, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(p, pe, o);
        }

        if (np) {
            {p = (( np))-1;}
            p--;
            {p++; cs = 2; goto _out;}
        }
        else {
            error(__FUNCTION__, *p);
            {p = (( pe))-1;}
        }
    }
	goto st2;
tr6:
#line 80 "edn_parser.rl"
	{
        // tokens with a leading ':'
        const char *np = parse_keyword(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr7:
#line 118 "edn_parser.rl"
	{
        // user identifiers and reserved keywords (true, false, nil)
        std::string sym;
        const char *np = parse_symbol(p, pe, sym);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {
            if      (sym == "true")  { o = Qtrue; }
            else if (sym == "false") { o = Qfalse; }
            else if (sym == "nil")   { o = Qnil; }
            else {
                o = Parser::make_edn_symbol(sym);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
tr8:
#line 133 "edn_parser.rl"
	{
        // [
        const char *np = parse_vector(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr9:
#line 112 "edn_parser.rl"
	{
        // tokens w/ leading \ (escaped characters \newline, \c, etc.)
        const char *np = parse_esc_char(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr10:
#line 145 "edn_parser.rl"
	{
        // {
        const char *np = parse_map(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
#line 206 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 179 "edn_parser.rl"

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
// string parsing - incoming string is raw so interpreting utf
// encodings & unicode values might be necessary. To optimize things a
// bit, we mark the string for encoding if anything outside of the
// ascii range is found.
//

#line 237 "edn_parser.cc"
static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;
static const int EDN_string_error = 0;

static const int EDN_string_en_main = 1;


#line 224 "edn_parser.rl"



const char* edn::Parser::parse_string(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "string";
    int cs;
    bool encode = false;
    const char *eof = pe;
    Rice::String s;


#line 259 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 237 "edn_parser.rl"
    p_save = p;

#line 267 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 34 )
		goto st2;
	goto st0;
tr3:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 286 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr2:
#line 213 "edn_parser.rl"
	{
        encode = true;
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 300 "edn_parser.cc"
	switch( (*p) ) {
		case 34: goto tr4;
		case 92: goto tr5;
	}
	if ( (*p) > -11 ) {
		if ( 0 <= (*p) && (*p) <= 31 )
			goto tr3;
	} else if ( (*p) >= -62 )
		goto tr2;
	goto st2;
tr4:
#line 205 "edn_parser.rl"
	{
        if (Parser::parse_byte_stream(p_save + 1, p, s, encode)) {
            {p = (( p + 1))-1;}
        } else {
            p--; {p++; cs = 8; goto _out;}
        }
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 327 "edn_parser.cc"
	goto st0;
tr5:
#line 213 "edn_parser.rl"
	{
        encode = true;
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 339 "edn_parser.cc"
	switch( (*p) ) {
		case 34: goto tr2;
		case 47: goto tr2;
		case 92: goto tr2;
		case 98: goto tr2;
		case 102: goto tr2;
		case 110: goto tr2;
		case 114: goto tr2;
		case 116: goto tr2;
		case 117: goto tr6;
	}
	if ( 0 <= (*p) && (*p) <= 31 )
		goto st0;
	goto st2;
tr6:
#line 213 "edn_parser.rl"
	{
        encode = true;
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 364 "edn_parser.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr7;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr7;
	} else
		goto tr7;
	goto st0;
tr7:
#line 213 "edn_parser.rl"
	{
        encode = true;
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 384 "edn_parser.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr8;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr8:
#line 213 "edn_parser.rl"
	{
        encode = true;
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 404 "edn_parser.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr9;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr9;
	} else
		goto tr9;
	goto st0;
tr9:
#line 213 "edn_parser.rl"
	{
        encode = true;
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 424 "edn_parser.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr2;
	} else
		goto tr2;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 456 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 239 "edn_parser.rl"

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
// keyword parsing
//

#line 482 "edn_parser.cc"
static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 3;
static const int EDN_keyword_error = 0;

static const int EDN_keyword_en_main = 1;


#line 269 "edn_parser.rl"



const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 499 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 277 "edn_parser.rl"
    p_save = p;

#line 507 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 58 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 35 )
		goto st3;
	if ( (*p) < 65 ) {
		if ( 46 <= (*p) && (*p) <= 47 )
			goto st3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st3;
	} else
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 33: goto st3;
		case 95: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else if ( (*p) >= 35 )
			goto st3;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st3;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st3;
		} else
			goto st3;
	} else
		goto st3;
	goto tr3;
tr3:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 569 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 279 "edn_parser.rl"

    if (cs >= EDN_keyword_first_final) {
        std::string buf;
        uint32_t len = p - p_save;
        // don't include leading ':' because Rice::Symbol will handle it
        buf.append(p_save + 1, len - 1);
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
// decimal parsing machine
//

#line 604 "edn_parser.cc"
static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 9;

static const int EDN_decimal_en_main = 1;


#line 312 "edn_parser.rl"



const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 620 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 320 "edn_parser.rl"
    p_save = p;

#line 628 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43: goto st2;
		case 45: goto st2;
		case 48: goto st3;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 48 )
		goto st3;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 46: goto st9;
		case 69: goto st6;
		case 101: goto st6;
	}
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st4;
		case 77: goto st12;
		case 101: goto st4;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st9;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
tr10:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 689 "edn_parser.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 43: goto st5;
		case 45: goto st5;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st0;
		case 77: goto st12;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st11;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st0;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 43: goto st7;
		case 45: goto st7;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 43: goto st0;
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st13;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr10;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 46: goto st9;
		case 69: goto st6;
		case 101: goto st6;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof12: cs = 12; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof13: cs = 13; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 322 "edn_parser.rl"

    if (cs >= EDN_decimal_first_final) {
        o = Parser::float_to_ruby(p_save, p - p_save);
        return p + 1;
    }
    else if (cs == EDN_decimal_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// integer parsing machine
//

#line 820 "edn_parser.cc"
static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 345 "edn_parser.rl"


const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 835 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 352 "edn_parser.rl"
    p_save = p;

#line 843 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43: goto st2;
		case 45: goto st2;
		case 48: goto st3;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st6;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 48 )
		goto st3;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st6;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 43: goto st0;
		case 45: goto st0;
	}
	if ( (*p) > 57 ) {
		if ( 77 <= (*p) && (*p) <= 78 )
			goto st5;
	} else if ( (*p) >= 48 )
		goto st0;
	goto tr4;
tr4:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 892 "edn_parser.cc"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 43: goto st0;
		case 45: goto st0;
	}
	if ( (*p) > 57 ) {
		if ( 77 <= (*p) && (*p) <= 78 )
			goto st0;
	} else if ( (*p) >= 48 )
		goto st0;
	goto tr4;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 43: goto st0;
		case 45: goto st0;
	}
	if ( (*p) > 57 ) {
		if ( 77 <= (*p) && (*p) <= 78 )
			goto st5;
	} else if ( (*p) >= 48 )
		goto st6;
	goto tr4;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 354 "edn_parser.rl"

    if (cs >= EDN_integer_first_final) {
        o = Parser::integer_to_ruby(p_save, p - p_save);
        return p + 1;
    }
    else if (cs == EDN_integer_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// operator parsing - handles tokens w/ a leading operator:
//
// 1. symbols w/ leading operator: -something, .somethingelse
// 2. number values w/ leading - or +
// 3. stand-alone operators: +, -, /, *, etc.
//

#line 953 "edn_parser.cc"
static const int EDN_operator_start = 1;
static const int EDN_operator_first_final = 3;
static const int EDN_operator_error = 0;

static const int EDN_operator_en_main = 1;


#line 424 "edn_parser.rl"



const char* edn::Parser::parse_operator(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 970 "edn_parser.cc"
	{
	cs = EDN_operator_start;
	}

#line 432 "edn_parser.rl"
    p_save = p;

#line 978 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st3;
		case 42: goto st3;
		case 43: goto st7;
		case 45: goto st7;
		case 46: goto st9;
		case 47: goto st3;
		case 95: goto st3;
	}
	if ( (*p) > 38 ) {
		if ( 60 <= (*p) && (*p) <= 63 )
			goto st3;
	} else if ( (*p) >= 36 )
		goto st3;
	goto st0;
st0:
cs = 0;
	goto _out;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st0;
		case 44: goto tr7;
		case 59: goto tr9;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 38 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr7;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else
			goto st0;
	} else
		goto st0;
	goto tr6;
tr6:
#line 411 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        std::string sym;
        sym += *(p_save);
        o = Parser::make_edn_symbol(sym);
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
tr10:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1052 "edn_parser.cc"
	goto st0;
tr5:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st5;
tr7:
#line 411 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        std::string sym;
        sym += *(p_save);
        o = Parser::make_edn_symbol(sym);
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr8:
#line 411 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        std::string sym;
        sym += *(p_save);
        o = Parser::make_edn_symbol(sym);
    }
#line 24 "edn_parser.rl"
	{ line_number++; }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr11:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr12:
#line 24 "edn_parser.rl"
	{ line_number++; }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1096 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr12;
		case 32: goto tr11;
		case 33: goto st0;
		case 44: goto tr11;
		case 59: goto tr13;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 38 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr11;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else
			goto st0;
	} else
		goto st0;
	goto tr10;
tr9:
#line 411 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        std::string sym;
        sym += *(p_save);
        o = Parser::make_edn_symbol(sym);
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr13:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1142 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr5;
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr5;
	goto st2;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st0;
		case 44: goto tr7;
		case 59: goto tr9;
		case 95: goto st0;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 36 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr7;
		} else if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 47 )
				goto st0;
		} else
			goto st0;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else
			goto tr15;
	} else
		goto tr14;
	goto tr6;
tr14:
#line 388 "edn_parser.rl"
	{
        // parse a number with the leading symbol - this is slightly
        // different than the one within EDN_value since it includes
        // the leading - or +
        //
        // try to parse a decimal first
        const char *np = parse_decimal(p_save, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(p_save, pe, o);
        }

        if (np) {
            {p = (( np))-1;}
            p--;
            {p++; cs = 8; goto _out;}
        }
        else {
            error(__FUNCTION__, *p);
            {p = (( pe))-1;}
        }
    }
	goto st8;
tr15:
#line 378 "edn_parser.rl"
	{
        // parse a symbol including the leading operator (-, +, .)
        std::string sym;
        const char *np = parse_symbol(p_save, pe, sym);
        if (np == NULL) { p--; {p++; cs = 8; goto _out;} } else {
            o = Parser::make_edn_symbol(sym);
            {p = (( np))-1;}
        }
    }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 1227 "edn_parser.cc"
	switch( (*p) ) {
		case 33: goto st0;
		case 95: goto st0;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st0;
		} else if ( (*p) >= 36 )
			goto st0;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else
			goto st0;
	} else
		goto st0;
	goto tr10;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st0;
		case 44: goto tr7;
		case 59: goto tr9;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 38 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr7;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr15;
		} else
			goto tr15;
	} else
		goto st0;
	goto tr6;
	}
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 3:
	case 7:
	case 9:
#line 411 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        std::string sym;
        sym += *(p_save);
        o = Parser::make_edn_symbol(sym);
    }
	break;
#line 1305 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 434 "edn_parser.rl"

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
// escaped char parsing - handles \c, \newline, \formfeed, etc.
//

#line 1331 "edn_parser.cc"
static const int EDN_escaped_char_start = 1;
static const int EDN_escaped_char_first_final = 4;
static const int EDN_escaped_char_error = 0;

static const int EDN_escaped_char_en_main = 1;


#line 463 "edn_parser.rl"



const char* edn::Parser::parse_esc_char(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 1348 "edn_parser.cc"
	{
	cs = EDN_escaped_char_start;
	}

#line 471 "edn_parser.rl"
    p_save = p;

#line 1356 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 92 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st4;
	} else if ( (*p) >= 65 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 10: goto tr7;
		case 32: goto tr6;
		case 44: goto tr6;
		case 59: goto tr8;
		case 92: goto st0;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr6;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st4;
	} else
		goto st4;
	goto tr5;
tr5:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1407 "edn_parser.cc"
	goto st0;
tr4:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st6;
tr6:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr7:
#line 24 "edn_parser.rl"
	{ line_number++; }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1427 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr7;
		case 32: goto tr6;
		case 44: goto tr6;
		case 59: goto tr8;
		case 92: goto st0;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr6;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st0;
	} else
		goto st0;
	goto tr5;
tr8:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1452 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr4;
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr4;
	goto st3;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 473 "edn_parser.rl"

    if (cs >= EDN_escaped_char_first_final) {
        // convert the escaped value to a character
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
// symbol parsing - handles identifiers that begin with an alpha
// character and an optional leading operator (name, -today,
// .yesterday)
//

#line 1501 "edn_parser.cc"
static const int EDN_symbol_start = 1;
static const int EDN_symbol_first_final = 5;
static const int EDN_symbol_error = 0;

static const int EDN_symbol_en_main = 1;


#line 507 "edn_parser.rl"



const char* edn::Parser::parse_symbol(const char *p, const char *pe, std::string& sym)
{
    int cs;


#line 1518 "edn_parser.cc"
	{
	cs = EDN_symbol_start;
	}

#line 515 "edn_parser.rl"
    p_save = p;

#line 1526 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st2;
		case 95: goto st2;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st2;
		} else if ( (*p) >= 36 )
			goto st2;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st2;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st5;
		} else
			goto st5;
	} else
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st5;
	} else if ( (*p) >= 65 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st0;
		case 35: goto st5;
		case 44: goto tr7;
		case 47: goto st4;
		case 59: goto tr10;
		case 95: goto st5;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 36 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr7;
		} else if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st0;
		} else
			goto st0;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st5;
		} else
			goto st5;
	} else
		goto st5;
	goto tr6;
tr6:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1611 "edn_parser.cc"
	goto st0;
tr4:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st7;
tr7:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
tr8:
#line 24 "edn_parser.rl"
	{ line_number++; }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1631 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st0;
		case 44: goto tr7;
		case 59: goto tr10;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 38 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr7;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr6;
tr10:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 1663 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr4;
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr4;
	goto st3;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st9;
	} else if ( (*p) >= 65 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st0;
		case 35: goto st9;
		case 44: goto tr7;
		case 47: goto st0;
		case 59: goto tr10;
		case 95: goto st9;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 36 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr7;
		} else if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st0;
		} else
			goto st0;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st9;
		} else
			goto st9;
	} else
		goto st9;
	goto tr6;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 517 "edn_parser.rl"

    if (cs >= EDN_symbol_first_final) {
        // copy the symbol text
        sym.clear();
        sym.append(p_save, p - p_save);
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
// EDN_sequence_common is used to parse EDN containers - elements are
// initially stored in a rice array and then the final corresponding
// container is built from the list (although, for vectors, lists, and
// sets the same array is used)
//

#line 572 "edn_parser.rl"


//
// vector-specific machine

#line 1764 "edn_parser.cc"
static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 588 "edn_parser.rl"



//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "vector";

    int cs;
    Rice::Array elems; // will store the vector's elements


#line 1787 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 602 "edn_parser.rl"

#line 1794 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 91 )
		goto st2;
	goto st0;
tr2:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 1813 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 544 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        Rice::Object e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                elems.push(e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1852 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st3;
		case 93: goto tr6;
		case 95: goto tr4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else if ( (*p) >= 65 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr3;
	goto st3;
tr6:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1892 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
	case 3:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 1913 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 603 "edn_parser.rl"

    if (cs >= EDN_vector_first_final) {
        o = elems;
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

#line 1940 "edn_parser.cc"
static const int EDN_list_start = 1;
static const int EDN_list_first_final = 4;
static const int EDN_list_error = 0;

static const int EDN_list_en_main = 1;


#line 632 "edn_parser.rl"


//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "list";

    int cs;
    Rice::Array elems;


#line 1962 "edn_parser.cc"
	{
	cs = EDN_list_start;
	}

#line 645 "edn_parser.rl"

#line 1969 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 40 )
		goto st2;
	goto st0;
tr2:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 1988 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 544 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        Rice::Object e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                elems.push(e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2027 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 41: goto tr5;
		case 44: goto st2;
		case 59: goto st3;
		case 95: goto tr4;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else if ( (*p) >= 65 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr5:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2059 "edn_parser.cc"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr3;
	goto st3;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
	case 3:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 2087 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 646 "edn_parser.rl"

    if (cs >= EDN_list_first_final) {
        o = elems;
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
// hash parsing
//

#line 2114 "edn_parser.cc"
static const int EDN_map_start = 1;
static const int EDN_map_first_final = 4;
static const int EDN_map_error = 0;

static const int EDN_map_en_main = 1;


#line 676 "edn_parser.rl"



const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "map";

    int cs;
    // since we don't know whether we're looking at a key or value,
    // initially store all elements in a list
    Rice::Array elems;


#line 2136 "edn_parser.cc"
	{
	cs = EDN_map_start;
	}

#line 689 "edn_parser.rl"

#line 2143 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 123 )
		goto st2;
	goto st0;
tr2:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 2162 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 544 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        Rice::Object e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                elems.push(e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2201 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st3;
		case 95: goto tr4;
		case 125: goto tr6;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else if ( (*p) >= 65 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr3;
	goto st3;
tr6:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2241 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
	case 3:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 2262 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 690 "edn_parser.rl"

    if (cs >= EDN_map_first_final) {

        // hash parsing is done. Make sure we have an even count
        if ((elems.size() % 2) != 0) {
            error(__FUNCTION__, "odd number of elements in map");
            return pe;
        }

        // now convert the sequence to a hash
        Rice::Hash rslt;
        while (elems.size())
        {
            Rice::Object k = elems.shift();
            rslt[k] = elems.shift();
        }

        o = rslt;
        return p + 1;
    }
    else if (cs == EDN_map_error) {
        return pe;
    }
    else if (cs == EDN_map_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// dispatch - handles all tokens with a leading #, then delegates to
// the corresponding machine. This machine consumes the # and passes
// the remaining data to the correct parser
//

#line 2305 "edn_parser.cc"
static const int EDN_dispatch_start = 1;
static const int EDN_dispatch_first_final = 2;
static const int EDN_dispatch_error = 0;

static const int EDN_dispatch_en_main = 1;


#line 754 "edn_parser.rl"



const char* edn::Parser::parse_dispatch(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 2322 "edn_parser.cc"
	{
	cs = EDN_dispatch_start;
	}

#line 762 "edn_parser.rl"

#line 2329 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 95: goto tr2;
		case 123: goto tr3;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 742 "edn_parser.rl"
	{
        // #inst, #uuid, or #user/tag
        const char *np = parse_tagged(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr2:
#line 736 "edn_parser.rl"
	{
        // discard token #_
        const char *np = parse_discard(p, pe);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr3:
#line 730 "edn_parser.rl"
	{
        // #{ }
        const char *np = parse_set(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2383 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 763 "edn_parser.rl"

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
// set parsing machine
//

#line 2411 "edn_parser.cc"
static const int EDN_set_start = 1;
static const int EDN_set_first_final = 4;
static const int EDN_set_error = 0;

static const int EDN_set_en_main = 1;


#line 792 "edn_parser.rl"


//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "set";

    int cs;
    Rice::Array elems; // stored as a vector


#line 2433 "edn_parser.cc"
	{
	cs = EDN_set_start;
	}

#line 805 "edn_parser.rl"

#line 2440 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 123 )
		goto st2;
	goto st0;
tr2:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 2459 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 544 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        Rice::Object e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                elems.push(e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2498 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st3;
		case 95: goto tr4;
		case 125: goto tr6;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else if ( (*p) >= 65 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr3;
	goto st3;
tr6:
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2538 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
	case 3:
#line 54 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 2559 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 806 "edn_parser.rl"

    if (cs >= EDN_set_first_final) {
        // all elements collected; now convert to a set
        o = Parser::make_ruby_set(elems);
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
// discard - consume the discard token and parse the next value to
// discard. TODO: perhaps optimize this so no object data is built
// by defining a new machine(s) to consume items within container
// delimiters
//

#line 2590 "edn_parser.cc"
static const int EDN_discard_start = 1;
static const int EDN_discard_first_final = 4;
static const int EDN_discard_error = 0;

static const int EDN_discard_en_main = 1;


#line 853 "edn_parser.rl"



const char* edn::Parser::parse_discard(const char *p, const char *pe)
{
    int cs;
    Rice::Object o;


#line 2608 "edn_parser.cc"
	{
	cs = EDN_discard_start;
	}

#line 862 "edn_parser.rl"

#line 2615 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 95 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr2:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2636 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr2;
		case 32: goto st2;
		case 40: goto tr3;
		case 44: goto st2;
		case 59: goto st3;
		case 95: goto tr3;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr3;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr3;
		} else if ( (*p) >= 65 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
tr3:
#line 836 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, o);
        if (np) {
            // this token is to be discard it so store it in the
            // discard stack - we really don't need to save it so this
            // could be simplified
            discard.push(o);
            {p = (( np))-1;}
        } else {
            p--; {p++; cs = 4; goto _out;}
        }
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2681 "edn_parser.cc"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr2;
	goto st3;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 863 "edn_parser.rl"

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
// tagged element parsing - #uuid, #inst, #{, #user/tag
//
// Current implementation expects a symbol followed by a value to
// match it against and does not check validity of uuid or rfc3339
// date characters.
//
// TODO:
// 1. need to check if we must support discard shenanigans such as
//
//    #symbol #_ discard data
//
// 2. add parse checks for uuid and inst for better error reporting
//

#line 2730 "edn_parser.cc"
static const int EDN_tagged_start = 1;
static const int EDN_tagged_first_final = 6;
static const int EDN_tagged_error = 0;

static const int EDN_tagged_en_main = 1;


#line 914 "edn_parser.rl"



const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o)
{
    std::string sym_name;
    Rice::Object data;

    int cs;


#line 2750 "edn_parser.cc"
	{
	cs = EDN_tagged_start;
	}

#line 925 "edn_parser.rl"

#line 2757 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 901 "edn_parser.rl"
	{
        // parses the symbol portion of the pair
        const char *np = parse_symbol(p, pe, sym_name);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else { {p = (( np))-1;} }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2785 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 35: goto tr5;
		case 40: goto tr4;
		case 44: goto st3;
		case 47: goto tr6;
		case 59: goto st4;
		case 95: goto tr5;
		case 123: goto tr4;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 33 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st3;
		} else if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr4;
		} else
			goto tr4;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr4;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 92 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr5;
			} else if ( (*p) >= 91 )
				goto tr4;
		} else
			goto tr5;
	} else
		goto tr5;
	goto st0;
tr3:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 2829 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 40: goto tr4;
		case 44: goto st3;
		case 59: goto st4;
		case 95: goto tr4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else if ( (*p) >= 65 )
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr4:
#line 906 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 6; goto _out;} } else { {p = (( np))-1;} }
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2867 "edn_parser.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr3;
	goto st4;
tr5:
#line 906 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 7; goto _out;} } else { {p = (( np))-1;} }
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 2890 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 35: goto tr5;
		case 40: goto tr4;
		case 44: goto st3;
		case 47: goto tr6;
		case 59: goto st4;
		case 95: goto tr5;
		case 123: goto tr4;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 33 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st3;
		} else if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr4;
		} else
			goto tr4;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr4;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 92 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr5;
			} else if ( (*p) >= 91 )
				goto tr4;
		} else
			goto tr5;
	} else
		goto tr5;
	goto st0;
tr6:
#line 906 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 8; goto _out;} } else { {p = (( np))-1;} }
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 2940 "edn_parser.cc"
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st5;
	} else if ( (*p) >= 65 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 35: goto tr8;
		case 40: goto tr4;
		case 44: goto st3;
		case 47: goto tr4;
		case 59: goto st4;
		case 95: goto tr8;
		case 123: goto tr4;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 33 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st3;
		} else if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr4;
		} else
			goto tr4;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr4;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 92 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr8;
			} else if ( (*p) >= 91 )
				goto tr4;
		} else
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr8:
#line 906 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 9; goto _out;} } else { {p = (( np))-1;} }
    }
#line 61 "edn_parser.rl"
	{ p--; {p++; cs = 9; goto _out;} }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 3000 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 35: goto tr8;
		case 40: goto tr4;
		case 44: goto st3;
		case 47: goto tr4;
		case 59: goto st4;
		case 95: goto tr8;
		case 123: goto tr4;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 33 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st3;
		} else if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr4;
		} else
			goto tr4;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr4;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 92 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr8;
			} else if ( (*p) >= 91 )
				goto tr4;
		} else
			goto tr8;
	} else
		goto tr8;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 926 "edn_parser.rl"

    if (cs >= EDN_tagged_first_final) {
        //std::cerr << __FUNCTION__ << " parse symbol name as '" << sym_name << "', value is: " << data << std::endl;

        try {
            // tagged_element makes a call to ruby which may throw an
            // exception when parsing the data
            o = Parser::tagged_element(sym_name, data);
        } catch (Rice::Exception& e) {
            error(__FUNCTION__, e.message().str());
            return pe;
        }
        return p + 1;
    }
    else if (cs == EDN_tagged_error) {
        return pe;
    }
    else if (cs == EDN_tagged_en_main) {} // silence ragel warning
    return NULL;
}




// ============================================================
// main parsing machine
//

#line 3079 "edn_parser.cc"
static const int EDN_start = 2;
static const int EDN_error = 0;

static const int EDN_en_main = 2;


#line 969 "edn_parser.rl"


//
// TODO: Currently using a sequence to handle cases with a discard
// but EDN's Reader allows token by token parsing
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


#line 3106 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 988 "edn_parser.rl"
    p = &buf[0];
    pe = p + len;
    eof = pe; // eof defined in Parser class

#line 3116 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr1:
#line 24 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 959 "edn_parser.rl"
	{
        const char* np = parse_value(p, pe, result);
        if (np == NULL) { {p = (( pe))-1;} {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 3137 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr1;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st1;
		case 95: goto tr4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else if ( (*p) >= 65 )
			goto tr4;
	} else
		goto tr4;
	goto st0;
st0:
cs = 0;
	goto _out;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( (*p) == 10 )
		goto tr1;
	goto st1;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof1: cs = 1; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 992 "edn_parser.rl"

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
