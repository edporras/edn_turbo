
#line 1 "edn_parser.rl"
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


#line 56 "edn_parser.rl"


// ============================================================
// machine for parsing various types
//


#line 30 "edn_parser.cc"
static const int EDN_value_start = 1;
static const int EDN_value_first_final = 11;
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 141 "edn_parser.rl"



const char *edn::Parser::parse_value(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 47 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 149 "edn_parser.rl"

#line 54 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 34: goto tr0;
		case 35: goto tr2;
		case 40: goto tr3;
		case 45: goto tr4;
		case 58: goto tr5;
		case 91: goto tr6;
		case 102: goto st2;
		case 110: goto st6;
		case 116: goto st8;
		case 123: goto tr10;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr4;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 83 "edn_parser.rl"
	{
        const char *np = parse_string(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr2:
#line 122 "edn_parser.rl"
	{
        const char *np = parse_tagged(p + 1, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr3:
#line 112 "edn_parser.rl"
	{
        const char *np = parse_list(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr4:
#line 88 "edn_parser.rl"
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
            {p++; cs = 11; goto _out;}
        }
        else {
            error(__FUNCTION__, *p);
            {p = (( pe))-1;}
        }
    }
	goto st11;
tr5:
#line 78 "edn_parser.rl"
	{
        const char *np = parse_keyword(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr6:
#line 107 "edn_parser.rl"
	{
        const char *np = parse_vector(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr10:
#line 117 "edn_parser.rl"
	{
        const char *np = parse_map(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 11; goto _out;} } else {p = (( np))-1;}
    }
	goto st11;
tr14:
#line 71 "edn_parser.rl"
	{
        o = Qfalse;
    }
	goto st11;
tr16:
#line 68 "edn_parser.rl"
	{
        o = Qnil;
    }
	goto st11;
tr19:
#line 74 "edn_parser.rl"
	{
        o = Qtrue;
    }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 127 "edn_parser.rl"
	{ p--; {p++; cs = 11; goto _out;} }
#line 166 "edn_parser.cc"
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 97 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 108 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 115 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 101 )
		goto tr14;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 105 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 108 )
		goto tr16;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 114 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 117 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 101 )
		goto tr19;
	goto st0;
	}
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 150 "edn_parser.rl"

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
// keyword parsing
//

#line 266 "edn_parser.cc"
static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 3;
static const int EDN_keyword_error = 0;

static const int EDN_keyword_en_main = 1;


#line 176 "edn_parser.rl"



const char* edn::Parser::parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 283 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 184 "edn_parser.rl"
    p_save = p;

#line 291 "edn_parser.cc"
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
		case 58: goto st3;
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
#line 173 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 365 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 186 "edn_parser.rl"

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

#line 399 "edn_parser.cc"
static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;
static const int EDN_string_error = 0;

static const int EDN_string_en_main = 1;


#line 231 "edn_parser.rl"



const char* edn::Parser::parse_string(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "string";
    int cs;
    const char *eof = pe;

    Rice::String s;

#line 419 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 242 "edn_parser.rl"
    p_save = p;

#line 427 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 446 "edn_parser.cc"
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
#line 213 "edn_parser.rl"
	{
        if (!parse_byte_stream(p_save + 1, p, s)) {
            p--;
            {p++; cs = 8; goto _out;}
        } else {
            {p = (( p + 1))-1;}
        }
    }
#line 222 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 478 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 563 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 244 "edn_parser.rl"

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

#line 587 "edn_parser.cc"
static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 8;

static const int EDN_decimal_en_main = 1;


#line 274 "edn_parser.rl"



const char* edn::Parser::parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 603 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 282 "edn_parser.rl"
    p_save = p;

#line 611 "edn_parser.cc"
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
		goto st7;
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
		goto st7;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 46: goto st4;
		case 69: goto st5;
		case 101: goto st5;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 69: goto st5;
		case 77: goto st11;
		case 101: goto st5;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st8;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr9;
tr9:
#line 265 "edn_parser.rl"
	{ p--; {p++; cs = 9; goto _out;} }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 677 "edn_parser.cc"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 43: goto st6;
		case 45: goto st6;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st10;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr9;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 69: goto st0;
		case 77: goto st0;
		case 101: goto st0;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st0;
	} else if ( (*p) >= 45 )
		goto st0;
	goto tr9;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 46: goto st4;
		case 69: goto st5;
		case 101: goto st5;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st7;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 284 "edn_parser.rl"

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

#line 771 "edn_parser.cc"
static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 306 "edn_parser.rl"


const char* edn::Parser::parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 786 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 313 "edn_parser.rl"
    p_save = p;

#line 794 "edn_parser.cc"
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
		goto st5;
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
		goto st5;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 77 )
		goto st0;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st0;
	goto tr4;
tr4:
#line 303 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 837 "edn_parser.cc"
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 77 )
		goto st3;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st5;
	goto tr4;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 315 "edn_parser.rl"

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

#line 350 "edn_parser.rl"


//
// vector-specific machine

#line 881 "edn_parser.cc"
static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 366 "edn_parser.rl"



//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "vector";

    int cs;
    Rice::Array arr;


#line 904 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 380 "edn_parser.rl"

#line 911 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 930 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 334 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            arr.push(v);
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 955 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st3;
		case 93: goto tr6;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 46 ) {
		if ( (*p) > 58 ) {
			if ( 91 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) >= 48 )
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
#line 345 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 998 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1018 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 381 "edn_parser.rl"

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

#line 1045 "edn_parser.cc"
static const int EDN_list_start = 1;
static const int EDN_list_first_final = 4;
static const int EDN_list_error = 0;

static const int EDN_list_en_main = 1;


#line 411 "edn_parser.rl"


//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "list";

    int cs;
    Rice::Array arr;


#line 1067 "edn_parser.cc"
	{
	cs = EDN_list_start;
	}

#line 424 "edn_parser.rl"

#line 1074 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 1093 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 334 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            arr.push(v);
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1118 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 41: goto tr5;
		case 44: goto st2;
		case 59: goto st3;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 46 ) {
		if ( (*p) > 58 ) {
			if ( 91 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) >= 48 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr5:
#line 345 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1154 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1181 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 425 "edn_parser.rl"

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

#line 1208 "edn_parser.cc"
static const int EDN_set_start = 1;
static const int EDN_set_first_final = 4;
static const int EDN_set_error = 0;

static const int EDN_set_en_main = 1;


#line 458 "edn_parser.rl"


//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "set";

    int cs;
    Rice::Array arr; // TODO: fix.. needs to be a Set


#line 1230 "edn_parser.cc"
	{
	cs = EDN_set_start;
	}

#line 471 "edn_parser.rl"

#line 1237 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
#line 1256 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 334 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            arr.push(v);
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1281 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st3;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
		case 125: goto tr6;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 46 ) {
		if ( (*p) > 58 ) {
			if ( 91 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) >= 48 )
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
#line 345 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1324 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
#line 1344 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 472 "edn_parser.rl"

    if (cs >= EDN_set_first_final) {
        o = arr;
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

#line 1371 "edn_parser.cc"
static const int EDN_map_start = 1;
static const int EDN_map_first_final = 6;
static const int EDN_map_error = 0;

static const int EDN_map_en_main = 1;


#line 531 "edn_parser.rl"



const char* edn::Parser::parse_map(const char *p, const char *pe, Rice::Object& o)
{
    static const char* EDN_TYPE = "map";

    int cs;
    Rice::Hash map;
    Rice::Object k, v;


#line 1392 "edn_parser.cc"
	{
	cs = EDN_map_start;
	}

#line 543 "edn_parser.rl"
    p_save = p;

#line 1400 "edn_parser.cc"
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
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	goto st0;
tr7:
#line 516 "edn_parser.rl"
	{
        error(__FUNCTION__, "map pair not found");
        {p = (( pe))-1;}
    }
	goto st0;
#line 1426 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr10:
#line 505 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; cs = 2; goto _out;}
        } else {
            map[k] = v;
            {p = (( np))-1;}
        }
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1450 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st2;
		case 40: goto tr4;
		case 44: goto st2;
		case 59: goto st5;
		case 102: goto tr4;
		case 110: goto tr4;
		case 116: goto tr4;
		case 123: goto tr4;
		case 125: goto tr6;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr4;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 46 ) {
		if ( (*p) > 58 ) {
			if ( 91 <= (*p) && (*p) <= 92 )
				goto tr4;
		} else if ( (*p) >= 48 )
			goto tr4;
	} else
		goto tr4;
	goto tr2;
tr9:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st3;
tr4:
#line 496 "edn_parser.rl"
	{
        const char *np = parse_value(p, pe, k);
        if (np == NULL) {
            p--; {p++; cs = 3; goto _out;}
        } else {
            {p = (( np))-1;}
        }
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1497 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr9;
		case 32: goto st3;
		case 40: goto tr10;
		case 44: goto st3;
		case 59: goto st4;
		case 102: goto tr10;
		case 110: goto tr10;
		case 116: goto tr10;
		case 123: goto tr10;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr10;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 46 ) {
		if ( (*p) > 58 ) {
			if ( 91 <= (*p) && (*p) <= 92 )
				goto tr10;
		} else if ( (*p) >= 48 )
			goto tr10;
	} else
		goto tr10;
	goto tr7;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr9;
	goto st4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 10 )
		goto tr3;
	goto st5;
tr6:
#line 521 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1546 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
#line 50 "edn_parser.rl"
	{
            std::stringstream s;
            s << "unterminated " << EDN_TYPE;
            error(__FUNCTION__, s.str());
            {p = (( pe))-1;}
        }
	break;
	case 3:
#line 516 "edn_parser.rl"
	{
        error(__FUNCTION__, "map pair not found");
        {p = (( pe))-1;}
    }
	break;
#line 1575 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 545 "edn_parser.rl"

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

#line 1600 "edn_parser.cc"
static const int EDN_tagged_start = 1;
static const int EDN_tagged_first_final = 10;
static const int EDN_tagged_error = 0;

static const int EDN_tagged_en_main = 1;


#line 596 "edn_parser.rl"



const char* edn::Parser::parse_tagged(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 1617 "edn_parser.cc"
	{
	cs = EDN_tagged_start;
	}

#line 604 "edn_parser.rl"
    p_save = p;


#line 1626 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 105: goto st2;
		case 117: goto st6;
		case 123: goto tr3;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 110 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 115 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 116 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 32 )
		goto tr7;
	goto st0;
tr3:
#line 582 "edn_parser.rl"
	{
        const char *np = parse_set(p, pe, o);
        if (np == NULL) { p--; {p++; cs = 10; goto _out;} } else {p = (( np))-1;}
    }
#line 587 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
tr7:
#line 577 "edn_parser.rl"
	{
        const char *np = parse_builtin_tagged(p + 1, pe, o, TAGGED_INST);
        if (np == NULL) { p--; {p++; cs = 10; goto _out;} } else {p = (( np))-1;}
    }
#line 587 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
tr11:
#line 572 "edn_parser.rl"
	{
        const char *np = parse_builtin_tagged(p + 1, pe, o, TAGGED_UUID);
        if (np == NULL) { p--; {p++; cs = 10; goto _out;} } else {p = (( np))-1;}
    }
#line 587 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 1701 "edn_parser.cc"
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 117 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 105 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 100 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 32 )
		goto tr11;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 607 "edn_parser.rl"

    if (cs >= EDN_tagged_first_final) {
        return p + 1;
    }
    else if (cs == EDN_tagged_error) {
        return pe;
    }
    else if (cs == EDN_tagged_en_main) {} // silence ragel warning
    return NULL;
}


// ============================================================
// built-in tagged element parsing - #uuid or #inst
//

#line 1763 "edn_parser.cc"
static const int EDN_builtin_tagged_start = 1;
static const int EDN_builtin_tagged_first_final = 5;
static const int EDN_builtin_tagged_error = 0;

static const int EDN_builtin_tagged_en_main = 1;


#line 635 "edn_parser.rl"



const char* edn::Parser::parse_builtin_tagged(const char *p, const char *pe, Rice::Object& o, TaggedType type)
{
    int cs;


#line 1780 "edn_parser.cc"
	{
	cs = EDN_builtin_tagged_start;
	}

#line 643 "edn_parser.rl"
    p_save = p;

#line 1788 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 34 )
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
		case 34: goto st5;
		case 43: goto st3;
		case 45: goto st2;
		case 46: goto st3;
		case 58: goto st3;
		case 84: goto st3;
		case 90: goto st3;
	}
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st4;
	} else if ( (*p) >= 48 )
		goto st2;
	goto st0;
tr5:
#line 628 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1828 "edn_parser.cc"
	switch( (*p) ) {
		case 32: goto st0;
		case 34: goto st0;
		case 43: goto st0;
		case 84: goto st0;
		case 90: goto st0;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st0;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 70 ) {
			if ( 97 <= (*p) && (*p) <= 102 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr5;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 34: goto st5;
		case 43: goto st3;
		case 84: goto st3;
		case 90: goto st3;
	}
	if ( (*p) > 46 ) {
		if ( 48 <= (*p) && (*p) <= 58 )
			goto st3;
	} else if ( (*p) >= 45 )
		goto st3;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 34: goto st5;
		case 45: goto st4;
	}
	if ( (*p) > 57 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st4;
	} else if ( (*p) >= 48 )
		goto st4;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 645 "edn_parser.rl"

    if (cs >= EDN_builtin_tagged_first_final) {
        std::size_t len = p - p_save;

        if (len > 2)
        {
            std::string buf;
            buf.reserve(len);

            // omit the quotes
            buf.append(p_save + 1, len - 2);

            if (type == TAGGED_INST) {
                o = get_rfc3339_date(buf);

                if (o.is_nil()) {
                    error(__FUNCTION__, "RFC3339 Date Format");
                    return pe;
                }
            }
            else {
                o = Rice::String(buf);
            }
            return p + 1;
        }
    }
    else if (cs == EDN_builtin_tagged_error) {
        error(__FUNCTION__, *p);
        return pe;
    }
    else if (cs == EDN_builtin_tagged_en_main) {} // silence ragel warning

    return NULL;
}


// ============================================================
// main parsing machine
//

#line 1929 "edn_parser.cc"
static const int EDN_start = 1;
static const int EDN_error = 0;

static const int EDN_en_main = 1;


#line 698 "edn_parser.rl"


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


#line 1952 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 713 "edn_parser.rl"
    p = &buf[0];
    pe = p + len;
    eof = pe; // eof defined in Parser class

#line 1962 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr2:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 1976 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr2;
		case 32: goto st1;
		case 40: goto tr3;
		case 44: goto st1;
		case 59: goto st3;
		case 102: goto tr3;
		case 110: goto tr3;
		case 116: goto tr3;
		case 123: goto tr3;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 34 <= (*p) && (*p) <= 35 )
				goto tr3;
		} else if ( (*p) >= 9 )
			goto st1;
	} else if ( (*p) > 46 ) {
		if ( (*p) > 58 ) {
			if ( 91 <= (*p) && (*p) <= 92 )
				goto tr3;
		} else if ( (*p) >= 48 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
st0:
cs = 0;
	goto _out;
tr6:
#line 22 "edn_parser.rl"
	{ line_number++; }
	goto st4;
tr3:
#line 690 "edn_parser.rl"
	{
        const char* np = parse_value(p, pe, result);
        if (np == NULL) { p--; {p++; cs = 4; goto _out;} } else {p = (( np))-1;}
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2021 "edn_parser.cc"
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

#line 717 "edn_parser.rl"

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
