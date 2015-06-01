
#line 1 "edn_parser.rl"
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


#line 66 "edn_parser.rl"


// ============================================================
// machine for parsing various types
//


#line 31 "edn_parser.cc"
static const int EDN_value_start = 1;
static const int EDN_value_first_final = 4;
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 164 "edn_parser.rl"



const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;


#line 49 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 173 "edn_parser.rl"

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
		case 42: goto tr0;
		case 45: goto tr5;
		case 58: goto tr6;
		case 91: goto tr7;
		case 92: goto st2;
		case 95: goto tr0;
		case 123: goto tr9;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 38 ) {
			if ( 46 <= (*p) && (*p) <= 47 )
				goto tr0;
		} else if ( (*p) >= 33 )
			goto tr0;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr0;
		} else
			goto tr0;
	} else
		goto tr5;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 78 "edn_parser.rl"
	{
        std::string sym;
        const char *np = parse_symbol(p, pe, sym);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {
            if      (sym == "true")  { o = Qtrue; }
            else if (sym == "false") { o = Qfalse; }
            else if (sym == "nil")   { o = Qnil; }
            else {
                o = Parser::make_edn_symbol(sym);
            }
            {p = (( np))-1;}
        }
    }
	goto st4;
tr2:
#line 107 "edn_parser.rl"
	{
        const char *np = parse_string(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
tr3:
#line 146 "edn_parser.rl"
	{
        const char *np = parse_dispatch(p + 1, pe, o);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
tr4:
#line 136 "edn_parser.rl"
	{
        const char *np = parse_list(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
tr5:
#line 112 "edn_parser.rl"
	{
        // try to parse a decimal first
        const char *np = parse_decimal(p, pe, o);
        if (np == NULL) {
            // if we can't, try to parse it as an int
            np = parse_integer(p, pe, o);
        }

        if (np) {
            {p = (( np))-1;}
            p--;
            {p++; cs = 4; goto _out;}
        }
        else {
            error(__FUNCTION__, *p);
            {p = (( pe))-1;}
        }
    }
	goto st4;
tr6:
#line 92 "edn_parser.rl"
	{
        const char *np = parse_keyword(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
tr7:
#line 131 "edn_parser.rl"
	{
        const char *np = parse_vector(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
tr9:
#line 141 "edn_parser.rl"
	{
        const char *np = parse_map(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 151 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
#line 181 "edn_parser.cc"
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( 33 <= (*p) && (*p) <= 126 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 151 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
#line 196 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr14;
		case 32: goto tr13;
		case 44: goto tr13;
		case 59: goto tr15;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr13;
	goto st0;
tr12:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st6;
tr13:
#line 97 "edn_parser.rl"
	{
        Rice::String s;

        if (!parse_escaped_char(*p, s)) {
            p--; {p++; cs = 6; goto _out;}
        } else {
            o = s;
        }
    }
	goto st6;
tr14:
#line 97 "edn_parser.rl"
	{
        Rice::String s;

        if (!parse_escaped_char(*p, s)) {
            p--; {p++; cs = 6; goto _out;}
        } else {
            o = s;
        }
    }
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 151 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
#line 242 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr12;
		case 32: goto st6;
		case 44: goto st6;
		case 59: goto st3;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st6;
	goto st0;
tr15:
#line 97 "edn_parser.rl"
	{
        Rice::String s;

        if (!parse_escaped_char(*p, s)) {
            p--; {p++; cs = 3; goto _out;}
        } else {
            o = s;
        }
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 268 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr12;
	goto st3;
	}
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 5:
#line 97 "edn_parser.rl"
	{
        Rice::String s;

        if (!parse_escaped_char(*p, s)) {
            p--; {p++; cs = 0; goto _out;}
        } else {
            o = s;
        }
    }
	break;
#line 295 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 174 "edn_parser.rl"

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

#line 321 "edn_parser.cc"
static const int EDN_symbol_start = 1;
static const int EDN_symbol_first_final = 3;
static const int EDN_symbol_error = 0;

static const int EDN_symbol_en_main = 1;


#line 202 "edn_parser.rl"



const char* edn::Parser::parse_symbol(const char *p, const char *pe, std::string& s)
{
    //std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    int cs;


#line 339 "edn_parser.cc"
	{
	cs = EDN_symbol_start;
	}

#line 211 "edn_parser.rl"
    p_save = p;

#line 347 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st3;
		case 95: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else if ( (*p) >= 36 )
			goto st3;
	} else if ( (*p) > 47 ) {
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
st0:
cs = 0;
	goto _out;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr5;
		case 33: goto st3;
		case 44: goto tr5;
		case 59: goto tr7;
		case 95: goto st3;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 38 )
				goto st3;
		} else if ( (*p) >= 9 )
			goto tr5;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st3;
		} else if ( (*p) >= 65 )
			goto st3;
	} else
		goto st3;
	goto tr4;
tr4:
#line 197 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 414 "edn_parser.cc"
	goto st0;
tr3:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st5;
tr5:
#line 197 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr6:
#line 23 "edn_parser.rl"
	{ line_number++; }
#line 197 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 434 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr5;
		case 33: goto st0;
		case 44: goto tr5;
		case 59: goto tr7;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 38 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr5;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr4;
tr7:
#line 197 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 466 "edn_parser.cc"
	if ( (*p) == 10 )
		goto tr3;
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 10 )
		goto tr3;
	goto st2;
	}
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 213 "edn_parser.rl"

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

#line 513 "edn_parser.cc"
static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 3;
static const int EDN_keyword_error = 0;

static const int EDN_keyword_en_main = 1;


#line 250 "edn_parser.rl"



const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 530 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 258 "edn_parser.rl"
    p_save = p;

#line 538 "edn_parser.cc"
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
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else if ( (*p) >= 35 )
			goto st3;
	} else if ( (*p) > 47 ) {
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
#line 247 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 611 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 260 "edn_parser.rl"

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

#line 645 "edn_parser.cc"
static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;
static const int EDN_string_error = 0;

static const int EDN_string_en_main = 1;


#line 305 "edn_parser.rl"



const char* edn::Parser::parse_string(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "   -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "string";
    int cs;
    const char *eof = pe;

    Rice::String s;

#line 666 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 317 "edn_parser.rl"
    p_save = p;

#line 674 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 34 )
		goto st2;
	goto st0;
tr2:
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	goto st0;
#line 693 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 34: goto tr3;
		case 92: goto st3;
	}
	if ( 0 <= (*p) && (*p) <= 31 )
		goto tr2;
	goto st2;
tr3:
#line 287 "edn_parser.rl"
	{
        if (!parse_byte_stream(p_save + 1, p, s)) {
            p--;
            {p++; cs = 8; goto _out;}
        } else {
            {p = (( p + 1))-1;}
        }
    }
#line 296 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 725 "edn_parser.cc"
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 117 )
		goto st4;
	if ( 0 <= (*p) && (*p) <= 31 )
		goto st0;
	goto st2;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st5;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st5;
	} else
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st7;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st7;
	} else
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st2;
	} else
		goto st2;
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	break;
#line 810 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 319 "edn_parser.rl"

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

#line 834 "edn_parser.cc"
static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 9;

static const int EDN_decimal_en_main = 1;


#line 347 "edn_parser.rl"



const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 850 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 355 "edn_parser.rl"
    p_save = p;

#line 858 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
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
#line 340 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 918 "edn_parser.cc"
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

#line 357 "edn_parser.rl"

    if (cs >= EDN_decimal_first_final) {
        o = float_to_ruby(p_save, p - p_save);
        return p + 1;
    }
    else if (cs == EDN_decimal_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// integer parsing grammar
//

#line 1049 "edn_parser.cc"
static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 379 "edn_parser.rl"


const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 1064 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 386 "edn_parser.rl"
    p_save = p;

#line 1072 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
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
	if ( (*p) == 124 )
		goto tr6;
	if ( (*p) > 57 ) {
		if ( 77 <= (*p) && (*p) <= 78 )
			goto st5;
	} else if ( (*p) >= 48 )
		goto st0;
	goto tr4;
tr4:
#line 376 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1118 "edn_parser.cc"
	goto st0;
tr6:
#line 376 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1128 "edn_parser.cc"
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
	if ( (*p) == 124 )
		goto tr6;
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

#line 388 "edn_parser.rl"

    if (cs >= EDN_integer_first_final) {
        o = integer_to_ruby(p_save, p - p_save);
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

#line 432 "edn_parser.rl"


//
// vector-specific machine

#line 1182 "edn_parser.cc"
static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 444 "edn_parser.rl"



//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    //std::cerr << __FUNCTION__ << "  -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "vector";

    int cs;
    Rice::Array arr;


#line 1206 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 459 "edn_parser.rl"

#line 1213 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	goto st0;
#line 1232 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 408 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
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
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1265 "edn_parser.cc"
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
#line 431 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1305 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	break;
#line 1325 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 460 "edn_parser.rl"

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

#line 1352 "edn_parser.cc"
static const int EDN_list_start = 1;
static const int EDN_list_first_final = 4;
static const int EDN_list_error = 0;

static const int EDN_list_en_main = 1;


#line 486 "edn_parser.rl"


//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "list";

    int cs;
    Rice::Array arr;


#line 1374 "edn_parser.cc"
	{
	cs = EDN_list_start;
	}

#line 499 "edn_parser.rl"

#line 1381 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	goto st0;
#line 1400 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 408 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
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
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1433 "edn_parser.cc"
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
#line 431 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1465 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	break;
#line 1492 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 500 "edn_parser.rl"

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

#line 1519 "edn_parser.cc"
static const int EDN_set_start = 1;
static const int EDN_set_first_final = 4;
static const int EDN_set_error = 0;

static const int EDN_set_en_main = 1;


#line 528 "edn_parser.rl"


//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << "     -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "set";

    int cs;
    Rice::Array arr; // store as a vector; then convert to a set once done


#line 1542 "edn_parser.cc"
	{
	cs = EDN_set_start;
	}

#line 542 "edn_parser.rl"

#line 1549 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	goto st0;
#line 1568 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 408 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
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
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1601 "edn_parser.cc"
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
#line 431 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1641 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	break;
#line 1661 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 543 "edn_parser.rl"

    if (cs >= EDN_set_first_final) {
        o = make_ruby_set(arr);
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

#line 1688 "edn_parser.cc"
static const int EDN_map_start = 1;
static const int EDN_map_first_final = 4;
static const int EDN_map_error = 0;

static const int EDN_map_en_main = 1;


#line 576 "edn_parser.rl"



const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    static const char* EDN_TYPE = "map";
    Rice::Array arr;
    int cs;


#line 1708 "edn_parser.cc"
	{
	cs = EDN_map_start;
	}

#line 587 "edn_parser.rl"

#line 1715 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	goto st0;
#line 1734 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 408 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
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
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1767 "edn_parser.cc"
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
#line 431 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1807 "edn_parser.cc"
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
#line 60 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            exit(-1);
        }
	break;
#line 1827 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 588 "edn_parser.rl"

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

#line 1865 "edn_parser.cc"
static const int EDN_tagged_start = 1;
static const int EDN_tagged_first_final = 5;
static const int EDN_tagged_error = 0;

static const int EDN_tagged_en_main = 1;


#line 643 "edn_parser.rl"



const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << " p '" << p << "'" << std::endl;
    std::string sym_name;
    Rice::Object object;

    int cs;


#line 1886 "edn_parser.cc"
	{
	cs = EDN_tagged_start;
	}

#line 655 "edn_parser.rl"

#line 1893 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto tr0;
		case 95: goto tr0;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 38 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr0;
		} else if ( (*p) >= 36 )
			goto tr0;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr0;
		} else
			goto tr0;
	} else
		goto tr0;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 631 "edn_parser.rl"
	{
        const char *np = parse_symbol(p, pe, sym_name);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else { {p = (( np))-1;} }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1936 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 34: goto tr5;
		case 40: goto tr5;
		case 44: goto st3;
		case 59: goto st4;
		case 95: goto tr4;
		case 123: goto tr5;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr4;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr4;
		} else
			goto tr5;
	} else
		goto tr4;
	goto st0;
tr3:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1973 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 40: goto tr5;
		case 44: goto st3;
		case 59: goto st4;
		case 95: goto tr5;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr5;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr5;
		} else if ( (*p) >= 65 )
			goto tr5;
	} else
		goto tr5;
	goto st0;
tr5:
#line 635 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, object);
        if (np == NULL) { p--; {p++; cs = 5; goto _out;} } else { {p = (( np))-1;} }
    }
#line 640 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 2010 "edn_parser.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr3;
	goto st4;
tr4:
#line 635 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, object);
        if (np == NULL) { p--; {p++; cs = 6; goto _out;} } else { {p = (( np))-1;} }
    }
#line 640 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2032 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 34: goto tr5;
		case 40: goto tr5;
		case 44: goto st3;
		case 59: goto st4;
		case 95: goto tr4;
		case 123: goto tr5;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 38 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 91 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr4;
		} else if ( (*p) > 92 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr4;
		} else
			goto tr5;
	} else
		goto tr4;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 656 "edn_parser.rl"

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

#line 2091 "edn_parser.cc"
static const int EDN_discard_start = 1;
static const int EDN_discard_first_final = 4;
static const int EDN_discard_error = 0;

static const int EDN_discard_en_main = 1;


#line 693 "edn_parser.rl"



const char* edn::Parser::parse_discard(const char *p, const char *pe)
{
    //std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    int cs;
    Rice::Object o;


#line 2110 "edn_parser.cc"
	{
	cs = EDN_discard_start;
	}

#line 703 "edn_parser.rl"

#line 2117 "edn_parser.cc"
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
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2138 "edn_parser.cc"
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
#line 681 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else { discard.push(o); {p = (( np))-1;} }
    }
#line 686 "edn_parser.rl"
	{
        p--; {p++; cs = 4; goto _out;}
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2177 "edn_parser.cc"
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

#line 704 "edn_parser.rl"

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

#line 2215 "edn_parser.cc"
static const int EDN_dispatch_start = 1;
static const int EDN_dispatch_first_final = 2;
static const int EDN_dispatch_error = 0;

static const int EDN_dispatch_en_main = 1;


#line 753 "edn_parser.rl"



const char* edn::Parser::parse_dispatch(const char *p, const char *pe, Rice::Object& o)
{
    //    std::cerr << __FUNCTION__ << " -  p: '" << p << "'" << std::endl;
    int cs;


#line 2233 "edn_parser.cc"
	{
	cs = EDN_dispatch_start;
	}

#line 762 "edn_parser.rl"

#line 2240 "edn_parser.cc"
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
#line 740 "edn_parser.rl"
	{
        //        std::cerr << "--- DISPATCH TAGGED: fpc is '" << fpc << "'" << std::endl;
        const char *np = parse_tagged(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 746 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr2:
#line 728 "edn_parser.rl"
	{
        //        std::cerr << "--- DISPATCH DISCARD: fpc is '" << fpc << "'" << std::endl;
        const char *np = parse_discard(p, pe);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 746 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr3:
#line 734 "edn_parser.rl"
	{
        //        std::cerr << "--- DISPATCH SET: fpc is '" << fpc << "'" << std::endl;
        const char *np = parse_set(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
#line 746 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2294 "edn_parser.cc"
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
// main parsing machine
//

#line 2321 "edn_parser.cc"
static const int EDN_start = 2;
static const int EDN_error = 0;

static const int EDN_en_main = 2;


#line 796 "edn_parser.rl"


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


#line 2348 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 815 "edn_parser.rl"
    p = &buf[0];
    pe = p + len;
    eof = pe; // eof defined in Parser class

#line 2358 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr1:
#line 23 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 785 "edn_parser.rl"
	{
        const char* np = parse_value(p, pe, result);
        if (np == NULL) { {p = (( pe))-1;} {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 2379 "edn_parser.cc"
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

#line 819 "edn_parser.rl"

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
