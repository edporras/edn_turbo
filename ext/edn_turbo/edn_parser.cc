
#line 1 "edn_parser.rl"
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "edn_parser.h"

//
// EDN spec at: https://github.com/edn-format/edn
//
//
// many thanks to Florian Frank for json-ruby which was essential in
// helping me learn about ragel
//


#line 54 "edn_parser.rl"


// ============================================================
// machine for parsing various EDN token types
//


#line 28 "edn_parser.cc"
static const int EDN_value_start = 1;
static const int EDN_value_first_final = 2;
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 170 "edn_parser.rl"



const char *edn::Parser::parse_value(const char *p, const char *pe, VALUE& v)
{
    int cs;


#line 45 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 178 "edn_parser.rl"

#line 52 "edn_parser.cc"
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
		case 94: goto tr10;
		case 95: goto tr0;
		case 123: goto tr11;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 42 ) {
			if ( 33 <= (*p) && (*p) <= 39 )
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
#line 98 "edn_parser.rl"
	{
        // stand-alone operators *, +, -, etc.
        const char *np = parse_operator(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr2:
#line 66 "edn_parser.rl"
	{
        // string types within double-quotes
        const char *np = parse_string(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr3:
#line 150 "edn_parser.rl"
	{
        // handles tokens w/ leading # ("#_", "#{", and tagged elems)
        const char *np = parse_dispatch(p + 1, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr4:
#line 132 "edn_parser.rl"
	{
        // (
        const char *np = parse_list(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr5:
#line 78 "edn_parser.rl"
	{
        // tokens w/ leading digits: non-negative integers & decimals.
        // try to parse a decimal first
        const char *np = parse_decimal(p, pe, v);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(p, pe, v);
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
#line 72 "edn_parser.rl"
	{
        // tokens with a leading ':'
        const char *np = parse_keyword(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr7:
#line 110 "edn_parser.rl"
	{
        // user identifiers and reserved keywords (true, false, nil)
        VALUE sym = Qnil;
        const char *np = parse_symbol(p, pe, sym);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {
            // parse_symbol will make 'sym' a ruby string
            if      (std::strcmp(RSTRING_PTR(sym), "true") == 0)  { v = Qtrue; }
            else if (std::strcmp(RSTRING_PTR(sym), "false") == 0) { v = Qfalse; }
            else if (std::strcmp(RSTRING_PTR(sym), "nil") == 0)   { v = Qnil; }
            else {
                v = Parser::make_edn_symbol(sym);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
tr8:
#line 126 "edn_parser.rl"
	{
        // [
        const char *np = parse_vector(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr9:
#line 104 "edn_parser.rl"
	{
        // tokens w/ leading \ (escaped characters \newline, \c, etc.)
        const char *np = parse_esc_char(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr10:
#line 144 "edn_parser.rl"
	{
        // ^
        const char *np = parse_meta(p, pe);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
tr11:
#line 138 "edn_parser.rl"
	{
        // {
        const char *np = parse_map(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
#line 212 "edn_parser.cc"
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

#line 243 "edn_parser.cc"
static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;
static const int EDN_string_error = 0;

static const int EDN_string_en_main = 1;


#line 224 "edn_parser.rl"



const char* edn::Parser::parse_string(const char *p, const char *pe, VALUE& v)
{
    static const char* EDN_TYPE = "string";
    int cs;
    bool encode = false;


#line 262 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 234 "edn_parser.rl"
    const char* p_save = p;

#line 270 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 289 "edn_parser.cc"
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
#line 303 "edn_parser.cc"
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
        if (Parser::parse_byte_stream(p_save + 1, p, v, encode)) {
            {p = (( p + 1))-1;}
        } else {
            p--; {p++; cs = 8; goto _out;}
        }
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 330 "edn_parser.cc"
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
#line 342 "edn_parser.cc"
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
#line 367 "edn_parser.cc"
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
#line 387 "edn_parser.cc"
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
#line 407 "edn_parser.cc"
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
#line 427 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 459 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 236 "edn_parser.rl"

    if (cs >= EDN_string_first_final) {
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

#line 484 "edn_parser.cc"
static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 3;
static const int EDN_keyword_error = 0;

static const int EDN_keyword_en_main = 1;


#line 266 "edn_parser.rl"



const char* edn::Parser::parse_keyword(const char *p, const char *pe, VALUE& v)
{
    int cs;


#line 501 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 274 "edn_parser.rl"
    const char* p_save = p;

#line 509 "edn_parser.cc"
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
	switch( (*p) ) {
		case 33: goto st3;
		case 95: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else if ( (*p) >= 35 )
			goto st3;
	} else if ( (*p) > 46 ) {
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
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 33: goto st3;
		case 47: goto st5;
		case 95: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 583 "edn_parser.cc"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 33: goto st5;
		case 47: goto st0;
		case 95: goto st5;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st5;
		} else if ( (*p) >= 35 )
			goto st5;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st5;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st5;
		} else
			goto st5;
	} else
		goto st5;
	goto tr3;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 276 "edn_parser.rl"

    if (cs >= EDN_keyword_first_final) {
        std::string buf;
        uint32_t len = p - p_save;
        // don't include leading ':' because the ruby symbol will handle it
        buf.append(p_save + 1, len - 1);
        v = ID2SYM(rb_intern(buf.c_str()));
        return p;
    }
    else if (cs == EDN_keyword_error) {
        error(__FUNCTION__, "Invalid keyword", *p);
        return pe;
    }
    else if (cs == EDN_keyword_en_main) {} // silence ragel warning
    return NULL;
}



// ============================================================
// decimal parsing machine
//

#line 646 "edn_parser.cc"
static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 9;

static const int EDN_decimal_en_main = 1;


#line 309 "edn_parser.rl"



const char* edn::Parser::parse_decimal(const char *p, const char *pe, VALUE& v)
{
    int cs;


#line 662 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 317 "edn_parser.rl"
    const char* p_save = p;

#line 670 "edn_parser.cc"
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 731 "edn_parser.cc"
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

#line 319 "edn_parser.rl"

    if (cs >= EDN_decimal_first_final) {
        v = Parser::float_to_ruby(p_save, p - p_save);
        return p + 1;
    }
    else if (cs == EDN_decimal_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// integer parsing machine
//

#line 862 "edn_parser.cc"
static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 342 "edn_parser.rl"


const char* edn::Parser::parse_integer(const char *p, const char *pe, VALUE& v)
{
    int cs;


#line 877 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 349 "edn_parser.rl"
    const char* p_save = p;

#line 885 "edn_parser.cc"
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 934 "edn_parser.cc"
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

#line 351 "edn_parser.rl"

    if (cs >= EDN_integer_first_final) {
        v = Parser::integer_to_ruby(p_save, p - p_save);
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

#line 995 "edn_parser.cc"
static const int EDN_operator_start = 1;
static const int EDN_operator_first_final = 3;
static const int EDN_operator_error = 0;

static const int EDN_operator_en_main = 1;


#line 421 "edn_parser.rl"



const char* edn::Parser::parse_operator(const char *p, const char *pe, VALUE& v)
{
    int cs;


#line 1012 "edn_parser.cc"
	{
	cs = EDN_operator_start;
	}

#line 429 "edn_parser.rl"
    const char* p_save = p;

#line 1020 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st3;
		case 42: goto st3;
		case 43: goto st8;
		case 45: goto st8;
		case 95: goto st3;
	}
	if ( (*p) < 46 ) {
		if ( 36 <= (*p) && (*p) <= 39 )
			goto st3;
	} else if ( (*p) > 47 ) {
		if ( 60 <= (*p) && (*p) <= 63 )
			goto st3;
	} else
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
		case 10: goto tr7;
		case 32: goto tr6;
		case 33: goto tr8;
		case 44: goto tr6;
		case 59: goto tr9;
		case 95: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 36 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr6;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 47 )
				goto tr8;
		} else
			goto tr8;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 63 )
				goto tr8;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else
			goto tr8;
	} else
		goto st0;
	goto tr5;
tr5:
#line 408 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        char op[2] = { *p_save, 0 };
        VALUE sym = rb_str_new2(op);
        v = Parser::make_edn_symbol(sym);
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
tr10:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1098 "edn_parser.cc"
	goto st0;
tr4:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st5;
tr6:
#line 408 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        char op[2] = { *p_save, 0 };
        VALUE sym = rb_str_new2(op);
        v = Parser::make_edn_symbol(sym);
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr7:
#line 408 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        char op[2] = { *p_save, 0 };
        VALUE sym = rb_str_new2(op);
        v = Parser::make_edn_symbol(sym);
    }
#line 20 "edn_parser.rl"
	{ line_number++; }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr11:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr12:
#line 20 "edn_parser.rl"
	{ line_number++; }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1142 "edn_parser.cc"
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
			if ( 36 <= (*p) && (*p) <= 39 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr11;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr10;
tr9:
#line 408 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        char op[2] = { *p_save, 0 };
        VALUE sym = rb_str_new2(op);
        v = Parser::make_edn_symbol(sym);
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr13:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1185 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr4;
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr4;
	goto st2;
tr8:
#line 375 "edn_parser.rl"
	{
        // parse a symbol including the leading operator (-, +, .)
        VALUE sym = Qnil;
        const char *np = parse_symbol(p_save, pe, sym);
        if (np == NULL) { p--; {p++; cs = 7; goto _out;} } else {
            v = Parser::make_edn_symbol(sym);
            {p = (( np))-1;}
        }
    }
	goto st7;
tr14:
#line 385 "edn_parser.rl"
	{
        // parse a number with the leading symbol - this is slightly
        // different than the one within EDN_value since it includes
        // the leading - or +
        //
        // try to parse a decimal first
        const char *np = parse_decimal(p_save, pe, v);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(p_save, pe, v);
        }

        if (np) {
            {p = (( np))-1;}
            p--;
            {p++; cs = 7; goto _out;}
        }
        else {
            error(__FUNCTION__, *p);
            {p = (( pe))-1;}
        }
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1237 "edn_parser.cc"
	switch( (*p) ) {
		case 33: goto st0;
		case 95: goto st0;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st0;
		} else if ( (*p) >= 36 )
			goto st0;
	} else if ( (*p) > 58 ) {
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
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 10: goto tr7;
		case 32: goto tr6;
		case 33: goto tr8;
		case 44: goto tr6;
		case 59: goto tr9;
		case 95: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 36 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr6;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 47 )
				goto tr8;
		} else
			goto tr8;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 63 )
				goto tr8;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else
			goto tr8;
	} else
		goto tr14;
	goto tr5;
	}
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 3:
	case 8:
#line 408 "edn_parser.rl"
	{
        // stand-alone operators (-, +, /, ... etc)
        char op[2] = { *p_save, 0 };
        VALUE sym = rb_str_new2(op);
        v = Parser::make_edn_symbol(sym);
    }
	break;
#line 1316 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 431 "edn_parser.rl"

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

#line 1342 "edn_parser.cc"
static const int EDN_escaped_char_start = 1;
static const int EDN_escaped_char_first_final = 4;
static const int EDN_escaped_char_error = 0;

static const int EDN_escaped_char_en_main = 1;


#line 460 "edn_parser.rl"



const char* edn::Parser::parse_esc_char(const char *p, const char *pe, VALUE& v)
{
    int cs;


#line 1359 "edn_parser.cc"
	{
	cs = EDN_escaped_char_start;
	}

#line 468 "edn_parser.rl"
    const char* p_save = p;

#line 1367 "edn_parser.cc"
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1418 "edn_parser.cc"
	goto st0;
tr4:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st6;
tr6:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr7:
#line 20 "edn_parser.rl"
	{ line_number++; }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1438 "edn_parser.cc"
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1463 "edn_parser.cc"
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

#line 470 "edn_parser.rl"

    if (cs >= EDN_escaped_char_first_final) {
        // convert the escaped value to a character
        if (!Parser::parse_escaped_char(p_save + 1, p, v)) {
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
//

#line 1513 "edn_parser.cc"
static const int EDN_symbol_start = 1;
static const int EDN_symbol_first_final = 4;
static const int EDN_symbol_error = 0;

static const int EDN_symbol_en_main = 1;


#line 515 "edn_parser.rl"



const char* edn::Parser::parse_symbol(const char *p, const char *pe, VALUE& s)
{
    int cs;


#line 1530 "edn_parser.cc"
	{
	cs = EDN_symbol_start;
	}

#line 523 "edn_parser.rl"
    const char* p_save = p;

#line 1538 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st4;
		case 42: goto st4;
		case 43: goto st10;
		case 47: goto st4;
		case 95: goto st4;
	}
	if ( (*p) < 60 ) {
		if ( (*p) > 39 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st10;
		} else if ( (*p) >= 36 )
			goto st4;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st4;
		} else if ( (*p) >= 65 )
			goto st4;
	} else
		goto st4;
	goto st0;
st0:
cs = 0;
	goto _out;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto tr8;
		case 33: goto st4;
		case 44: goto tr8;
		case 47: goto st3;
		case 59: goto tr11;
		case 95: goto st4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st4;
		} else if ( (*p) >= 9 )
			goto tr8;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st4;
		} else if ( (*p) >= 65 )
			goto st4;
	} else
		goto st4;
	goto tr7;
tr7:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1606 "edn_parser.cc"
	goto st0;
tr4:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st6;
tr8:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr9:
#line 20 "edn_parser.rl"
	{ line_number++; }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1626 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto tr8;
		case 33: goto st0;
		case 44: goto tr8;
		case 59: goto tr11;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr8;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr7;
tr11:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1658 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr4;
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr4;
	goto st2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 33: goto st8;
		case 42: goto st8;
		case 43: goto st9;
		case 47: goto st8;
		case 95: goto st8;
	}
	if ( (*p) < 60 ) {
		if ( (*p) > 39 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st9;
		} else if ( (*p) >= 36 )
			goto st8;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st8;
		} else if ( (*p) >= 65 )
			goto st8;
	} else
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto tr8;
		case 33: goto st8;
		case 44: goto tr8;
		case 47: goto st0;
		case 59: goto tr11;
		case 95: goto st8;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st8;
		} else if ( (*p) >= 9 )
			goto tr8;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st8;
		} else if ( (*p) >= 65 )
			goto st8;
	} else
		goto st8;
	goto tr7;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto tr8;
		case 33: goto st8;
		case 35: goto st0;
		case 44: goto tr8;
		case 59: goto tr11;
		case 95: goto st8;
	}
	if ( (*p) < 47 ) {
		if ( (*p) < 36 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr8;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 46 )
				goto st8;
		} else
			goto st8;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st8;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st8;
		} else
			goto st8;
	} else
		goto st0;
	goto tr7;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto tr8;
		case 33: goto st4;
		case 35: goto st0;
		case 44: goto tr8;
		case 47: goto st3;
		case 59: goto tr11;
		case 95: goto st4;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 36 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr8;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 46 )
				goto st4;
		} else
			goto st4;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st4;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st4;
		} else
			goto st4;
	} else
		goto st0;
	goto tr7;
	}
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 525 "edn_parser.rl"

    if (cs >= EDN_symbol_first_final) {
        // copy the symbol text
        if (s == Qnil)
            s = rb_str_new2("");
        rb_str_cat(s, p_save, p - p_save);
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
// initially stored in an array and then the final corresponding
// container is built from the list (although, for vectors, lists, and
// sets the same array is used)
//

#line 581 "edn_parser.rl"


//
// vector-specific machine

#line 1839 "edn_parser.cc"
static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 597 "edn_parser.rl"



//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, VALUE& v)
{
    static const char* EDN_TYPE = "vector";

    int cs;
    VALUE elems = rb_ary_new(); // will store the vector's elements


#line 1862 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 611 "edn_parser.rl"

#line 1869 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 1888 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 553 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        VALUE e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop_back();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                rb_ary_push(elems, e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1927 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st3;
		case 93: goto tr6;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 95 ) {
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1965 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 1986 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 612 "edn_parser.rl"

    if (cs >= EDN_vector_first_final) {
        v = elems;
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

#line 2013 "edn_parser.cc"
static const int EDN_list_start = 1;
static const int EDN_list_first_final = 4;
static const int EDN_list_error = 0;

static const int EDN_list_en_main = 1;


#line 641 "edn_parser.rl"


//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, VALUE& v)
{
    static const char* EDN_TYPE = "list";

    int cs;
    VALUE elems = rb_ary_new();


#line 2035 "edn_parser.cc"
	{
	cs = EDN_list_start;
	}

#line 654 "edn_parser.rl"

#line 2042 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 2061 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 553 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        VALUE e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop_back();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                rb_ary_push(elems, e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2100 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 41: goto tr5;
		case 44: goto st2;
		case 59: goto st3;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 63 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 92 ) {
		if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else if ( (*p) >= 94 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr5:
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2131 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 2159 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 655 "edn_parser.rl"

    if (cs >= EDN_list_first_final) {
        v = elems;
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

#line 2186 "edn_parser.cc"
static const int EDN_map_start = 1;
static const int EDN_map_first_final = 4;
static const int EDN_map_error = 0;

static const int EDN_map_en_main = 1;


#line 685 "edn_parser.rl"



const char* edn::Parser::parse_map(const char *p, const char *pe, VALUE& v)
{
    static const char* EDN_TYPE = "map";

    int cs;
    // since we don't know whether we're looking at a key or value,
    // initially store all elements in a list
    VALUE elems = rb_ary_new();


#line 2208 "edn_parser.cc"
	{
	cs = EDN_map_start;
	}

#line 698 "edn_parser.rl"

#line 2215 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 2234 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 553 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        VALUE e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop_back();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                rb_ary_push(elems, e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2273 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st3;
		case 125: goto tr6;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2314 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 2335 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 699 "edn_parser.rl"

    if (cs >= EDN_map_first_final) {

        // hash parsing is done. Make sure we have an even count
        if ((RARRAY_LEN(elems) % 2) != 0) {
            error(__FUNCTION__, "odd number of elements in map");
            return pe;
        }

        // now convert the sequence to a hash
        VALUE rslt = rb_hash_new();
        while (RARRAY_LEN(elems) > 0)
        {
            VALUE k = rb_ary_shift(elems);
            rb_hash_aset(rslt, k, rb_ary_shift(elems));
        }

        v = rslt;
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

#line 2378 "edn_parser.cc"
static const int EDN_dispatch_start = 1;
static const int EDN_dispatch_first_final = 2;
static const int EDN_dispatch_error = 0;

static const int EDN_dispatch_en_main = 1;


#line 763 "edn_parser.rl"



const char* edn::Parser::parse_dispatch(const char *p, const char *pe, VALUE& v)
{
    int cs;


#line 2395 "edn_parser.cc"
	{
	cs = EDN_dispatch_start;
	}

#line 771 "edn_parser.rl"

#line 2402 "edn_parser.cc"
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
#line 751 "edn_parser.rl"
	{
        // #inst, #uuid, or #user/tag
        const char *np = parse_tagged(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr2:
#line 745 "edn_parser.rl"
	{
        // discard token #_
        const char *np = parse_discard(p, pe);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr3:
#line 739 "edn_parser.rl"
	{
        // #{ }
        const char *np = parse_set(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2456 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 772 "edn_parser.rl"

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

#line 2484 "edn_parser.cc"
static const int EDN_set_start = 1;
static const int EDN_set_first_final = 4;
static const int EDN_set_error = 0;

static const int EDN_set_en_main = 1;


#line 801 "edn_parser.rl"


//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, VALUE& v)
{
    static const char* EDN_TYPE = "set";

    int cs;
    VALUE elems = rb_ary_new(); // stored as an array


#line 2506 "edn_parser.cc"
	{
	cs = EDN_set_start;
	}

#line 814 "edn_parser.rl"

#line 2513 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	goto st0;
#line 2532 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 553 "edn_parser.rl"
	{
        // reads an item within a sequence (vector, list, map, or
        // set). Regardless of the sequence type, an array of the
        // items is built. Once done, the sequence parser will convert
        // if needed
        VALUE e;
        const char *np = parse_value(p, pe, e);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            // if there's an entry in the discard list, the current
            // object is not meant to be kept due to a #_ so don't
            // push it into the list of elements
            if (!discard.empty()) {
                discard.pop_back();
            }
            else {
                // otherwise we add it to the list of elements for the
                // corresponding container
                rb_ary_push(elems, e);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2571 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st3;
		case 125: goto tr6;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
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
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2612 "edn_parser.cc"
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
#line 46 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            p--; {p++; cs = 0; goto _out;}
        }
	break;
#line 2633 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 815 "edn_parser.rl"

    if (cs >= EDN_set_first_final) {
        // all elements collected; now convert to a set
        v = Parser::make_ruby_set(elems);
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
// discard. TODO: perhaps optimize this so no object data is built by
// defining a machine to consume items within container delimiters
//

#line 2663 "edn_parser.cc"
static const int EDN_discard_start = 1;
static const int EDN_discard_first_final = 4;
static const int EDN_discard_error = 0;

static const int EDN_discard_en_main = 1;


#line 865 "edn_parser.rl"



const char* edn::Parser::parse_discard(const char *p, const char *pe)
{
    int cs;
    VALUE v;


#line 2681 "edn_parser.cc"
	{
	cs = EDN_discard_start;
	}

#line 874 "edn_parser.rl"

#line 2688 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 95 )
		goto st2;
	goto st0;
tr2:
#line 855 "edn_parser.rl"
	{
        std::stringstream s;
        s << "discard sequence without element to discard";
        error(__FUNCTION__, s.str());
        p--; {p++; cs = 0; goto _out;}
    }
	goto st0;
#line 2707 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2719 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st3;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr4:
#line 844 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {
            // this token is to be discarded so store it in the
            // discard stack - we really don't need to save it so this
            // could be simplified
            discard.push_back(v);
            {p = (( np))-1;}
        }
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2763 "edn_parser.cc"
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
#line 855 "edn_parser.rl"
	{
        std::stringstream s;
        s << "discard sequence without element to discard";
        error(__FUNCTION__, s.str());
        p--; {p++; cs = 0; goto _out;}
    }
	break;
#line 2790 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 875 "edn_parser.rl"

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

#line 2828 "edn_parser.cc"
static const int EDN_tagged_start = 1;
static const int EDN_tagged_first_final = 6;
static const int EDN_tagged_error = 0;

static const int EDN_tagged_en_main = 1;


#line 931 "edn_parser.rl"



const char* edn::Parser::parse_tagged(const char *p, const char *pe, VALUE& v)
{
    VALUE sym_name = Qnil;
    VALUE data = Qnil;

    int cs;


#line 2848 "edn_parser.cc"
	{
	cs = EDN_tagged_start;
	}

#line 942 "edn_parser.rl"

#line 2855 "edn_parser.cc"
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
#line 918 "edn_parser.rl"
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
#line 2883 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 44: goto st3;
		case 45: goto tr5;
		case 47: goto tr6;
		case 59: goto st4;
		case 94: goto tr4;
		case 95: goto tr5;
		case 123: goto tr4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr5;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr5;
		} else
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr3:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 2921 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 44: goto st3;
		case 59: goto st4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr4:
#line 923 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 6; goto _out;} } else { {p = (( np))-1;} }
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2960 "edn_parser.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr3;
	goto st4;
tr5:
#line 923 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 7; goto _out;} } else { {p = (( np))-1;} }
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 2983 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 44: goto st3;
		case 45: goto tr5;
		case 47: goto tr6;
		case 59: goto st4;
		case 94: goto tr4;
		case 95: goto tr5;
		case 123: goto tr4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr5;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr5;
		} else
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr6:
#line 923 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 8; goto _out;} } else { {p = (( np))-1;} }
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 3027 "edn_parser.cc"
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
		case 44: goto st3;
		case 45: goto tr8;
		case 59: goto st4;
		case 94: goto tr4;
		case 95: goto tr8;
		case 123: goto tr4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr8;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr8:
#line 923 "edn_parser.rl"
	{
        // parses the value portion
        const char *np = parse_value(p, pe, data);
        if (np == NULL) { p--; {p++; cs = 9; goto _out;} } else { {p = (( np))-1;} }
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 9; goto _out;} }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 3080 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 44: goto st3;
		case 45: goto tr8;
		case 59: goto st4;
		case 94: goto tr4;
		case 95: goto tr8;
		case 123: goto tr4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr8;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else
			goto tr4;
	} else
		goto tr4;
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

#line 943 "edn_parser.rl"

    if (cs >= EDN_tagged_first_final) {
        //std::cerr << __FUNCTION__ << " parse symbol name as '" << sym_name << "', value is: " << data << std::endl;

        try {
            // tagged_element makes a call to ruby which may throw an
            // exception when parsing the data
            v = Parser::tagged_element(sym_name, data);
        } catch (std::exception& e) {
            error(__FUNCTION__, e.what());
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
// metadata - looks like ruby just discards this but we'll track it
// and provide a means to retrive after each parse op - might be
// useful?
//

#line 3154 "edn_parser.cc"
static const int EDN_meta_start = 1;
static const int EDN_meta_first_final = 3;
static const int EDN_meta_error = 0;

static const int EDN_meta_en_main = 1;


#line 986 "edn_parser.rl"



const char* edn::Parser::parse_meta(const char *p, const char *pe)
{
    int cs;
    VALUE v;


#line 3172 "edn_parser.cc"
	{
	cs = EDN_meta_start;
	}

#line 995 "edn_parser.rl"

#line 3179 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 94 )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) < 60 ) {
		if ( (*p) < 42 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr2;
		} else if ( (*p) > 43 ) {
			if ( 45 <= (*p) && (*p) <= 58 )
				goto tr2;
		} else
			goto tr2;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr2;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr2;
		} else
			goto tr2;
	} else
		goto tr2;
	goto st0;
tr2:
#line 978 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, v);
        if (np == NULL) { p--; {p++; cs = 3; goto _out;} } else {p = (( np))-1;}
    }
#line 53 "edn_parser.rl"
	{ p--; {p++; cs = 3; goto _out;} }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 3230 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 996 "edn_parser.rl"

    if (cs >= EDN_meta_first_final) {
        metadata.push_back(v);
        return p + 1;
    }
    else if (cs == EDN_meta_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_meta_en_main) {} // silence ragel warning

    return NULL;
}



// ============================================================
// parses entire input but expects single valid token at the
// top-level, therefore, does not tokenize source stream
//

#line 3262 "edn_parser.cc"
static const int EDN_parser_start = 2;
static const int EDN_parser_first_final = 2;
static const int EDN_parser_error = 0;

static const int EDN_parser_en_main = 2;


#line 1045 "edn_parser.rl"



VALUE edn::Parser::parse(const char* src, std::size_t len)
{
    int cs;
    VALUE result = Qnil;


#line 3280 "edn_parser.cc"
	{
	cs = EDN_parser_start;
	}

#line 1054 "edn_parser.rl"
    set_source(src, len);

#line 3288 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr1:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 1022 "edn_parser.rl"
	{
        // save the count of metadata items before we parse this value
        // so we can determine if we've read another metadata value or
        // an actual data item
        std::size_t meta_size = metadata.size();
        const char* np = parse_value(p, pe, result);
        if (np == NULL) { {p = (( pe))-1;} {p++; cs = 2; goto _out;} } else {
            // if we have metadata saved and it matches the count we
            // saved before we parsed a value, then we must bind the
            // metadata sequence to it
            if (!metadata.empty() && metadata.size() == meta_size) {
                // this will empty the metadata sequence too
                result = bind_meta_to_value(result);
            }
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 3322 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr1;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st1;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr4;
		} else
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

#line 1056 "edn_parser.rl"

    if (cs == EDN_parser_error) {
        if (p)
            error(__FUNCTION__, *p);
        return EDNT_EOF;
    }
    else if (cs == EDN_parser_first_final) {
        p = pe = eof = NULL;
    }
    else if (cs == EDN_parser_en_main) {} // silence ragel warning
    return result;
}


// ============================================================
// token-by-token machine
//

#line 3384 "edn_parser.cc"
static const int EDN_tokens_start = 1;

static const int EDN_tokens_en_main = 1;


#line 1111 "edn_parser.rl"



//
//
edn::Parser::eTokenState edn::Parser::parse_next(VALUE& value)
{
    int cs;
    eTokenState state = TOKEN_ERROR;
    // need to track metadada read and bind it to the next value read
    // - but must account for sequences of metadata values
    std::size_t meta_size;

    // clear any previously saved discards; only track if read during
    // this op
    discard.clear();


#line 3409 "edn_parser.cc"
	{
	cs = EDN_tokens_start;
	}

#line 1129 "edn_parser.rl"

#line 3416 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr2:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 3430 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr2;
		case 32: goto st1;
		case 44: goto st1;
		case 59: goto st3;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr3;
		} else if ( (*p) >= 9 )
			goto st1;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr3;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr3;
		} else
			goto tr3;
	} else
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr6:
#line 20 "edn_parser.rl"
	{ line_number++; }
	goto st4;
tr3:
#line 1079 "edn_parser.rl"
	{
        // we won't know if we've parsed a discard or a metadata until
        // after parse_value() is done. Save the current number of
        // elements in the metadata sequence; then we can check if it
        // grew or if the discard sequence grew
        meta_size = metadata.size();

        const char* np = parse_value(p, pe, value);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {
            if (metadata.size() > 0) {
                // was an additional metadata entry read? if so, don't
                // return a value
                if (metadata.size() > meta_size) {
                    state = TOKEN_IS_META;
                }
                else {
                    // a value was read and there's a pending metadata
                    // sequence. Bind them.
                    value = bind_meta_to_value(value);
                    state = TOKEN_OK;
                }
            } else if (!discard.empty()) {
                // a discard read. Don't return a value
                state = TOKEN_IS_DISCARD;
            } else {
                state = TOKEN_OK;
            }
            {p = (( np))-1;}
        }
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 3499 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto st4;
		case 44: goto st4;
		case 59: goto st2;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st4;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr6;
	goto st2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr2;
	goto st3;
	}
	_test_eof1: cs = 1; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 1130 "edn_parser.rl"

    if (cs == EDN_tokens_en_main) {} // silence ragel warning
    return state;
}


/*
 * Local variables:
 * mode: c
 * c-file-style: ruby
 * indent-tabs-mode: nil
 * End:
 */
