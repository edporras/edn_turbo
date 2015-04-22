
#line 1 "edn_parser.rl"
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


#line 41 "edn_parser.rl"


// ============================================================
// machine for parsing various types
//


#line 30 "edn_parser.cc"
static const char _EDN_value_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1,
	3, 1, 4, 1, 5, 1, 6, 1,
	7
};

static const char _EDN_value_key_offsets[] = {
	0, 0, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18
};

static const char _EDN_value_trans_keys[] = {
	34, 45, 58, 91, 102, 110, 116, 48,
	57, 97, 108, 115, 101, 105, 108, 114,
	117, 101, 0
};

static const char _EDN_value_single_lengths[] = {
	0, 7, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 0
};

static const char _EDN_value_range_lengths[] = {
	0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0
};

static const char _EDN_value_index_offsets[] = {
	0, 0, 9, 11, 13, 15, 17, 19,
	21, 23, 25, 27
};

static const char _EDN_value_trans_targs[] = {
	11, 11, 11, 11, 2, 6, 8, 11,
	0, 3, 0, 4, 0, 5, 0, 11,
	0, 7, 0, 11, 0, 9, 0, 10,
	0, 11, 0, 0, 0
};

static const char _EDN_value_trans_actions[] = {
	9, 11, 7, 13, 0, 0, 0, 11,
	0, 0, 0, 0, 0, 0, 0, 3,
	0, 0, 0, 1, 0, 0, 0, 0,
	0, 5, 0, 0, 0
};

static const char _EDN_value_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 15
};

static const int EDN_value_start = 1;
static const int EDN_value_first_final = 11;

static const int EDN_value_en_main = 1;


#line 102 "edn_parser.rl"



const char *edn::Parser::EDN_parse_value(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;

    int cs;


#line 99 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 112 "edn_parser.rl"

#line 106 "edn_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _EDN_value_actions + _EDN_value_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 7:
#line 91 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 127 "edn_parser.cc"
		}
	}

	_keys = _EDN_value_trans_keys + _EDN_value_key_offsets[cs];
	_trans = _EDN_value_index_offsets[cs];

	_klen = _EDN_value_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _EDN_value_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _EDN_value_trans_targs[_trans];

	if ( _EDN_value_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _EDN_value_actions + _EDN_value_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 53 "edn_parser.rl"
	{
        o = Qnil;
    }
	break;
	case 1:
#line 56 "edn_parser.rl"
	{
        o = Qfalse;
    }
	break;
	case 2:
#line 59 "edn_parser.rl"
	{
        o = Qtrue;
    }
	break;
	case 3:
#line 63 "edn_parser.rl"
	{
        const char *np = EDN_parse_keyword(p, pe, o);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
	case 4:
#line 68 "edn_parser.rl"
	{
        const char *np = EDN_parse_string(p, pe, o);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
	case 5:
#line 73 "edn_parser.rl"
	{
        const char *np;

        np = EDN_parse_decimal(p, pe, o);
        if (np != NULL)
            {p = (( np))-1;}
        else {
            np = EDN_parse_integer(p, pe, o);
            if (np != NULL) {p = (( np))-1;}
        }
        p--; {p++; goto _out; }
    }
	break;
	case 6:
#line 86 "edn_parser.rl"
	{
        const char *np = EDN_parse_vector(p, pe, o);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
#line 245 "edn_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 113 "edn_parser.rl"

    if (cs >= EDN_value_first_final) {
        return p;
    }

    return NULL;
}


// ============================================================
// keyword parsing
//

#line 272 "edn_parser.cc"
static const char _EDN_keyword_actions[] = {
	0, 1, 0
};

static const char _EDN_keyword_key_offsets[] = {
	0, 0, 1, 6, 11, 20, 20
};

static const char _EDN_keyword_trans_keys[] = {
	58, 95, 65, 90, 97, 122, 95, 65,
	90, 97, 122, 39, 47, 95, 48, 57,
	65, 90, 97, 122, 39, 47, 95, 48,
	57, 65, 90, 97, 122, 0
};

static const char _EDN_keyword_single_lengths[] = {
	0, 1, 1, 1, 3, 0, 3
};

static const char _EDN_keyword_range_lengths[] = {
	0, 0, 2, 2, 3, 0, 3
};

static const char _EDN_keyword_index_offsets[] = {
	0, 0, 2, 6, 10, 17, 18
};

static const char _EDN_keyword_indicies[] = {
	0, 1, 2, 2, 2, 1, 3, 3,
	3, 1, 1, 5, 2, 2, 2, 2,
	4, 1, 1, 1, 3, 3, 3, 3,
	4, 0
};

static const char _EDN_keyword_trans_targs[] = {
	2, 0, 4, 6, 5, 3
};

static const char _EDN_keyword_trans_actions[] = {
	0, 0, 0, 0, 1, 0
};

static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 4;

static const int EDN_keyword_en_main = 1;


#line 134 "edn_parser.rl"



const char* edn::Parser::EDN_parse_keyword(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;


#line 331 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 143 "edn_parser.rl"
    p_save = p + 1;

#line 339 "edn_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _EDN_keyword_trans_keys + _EDN_keyword_key_offsets[cs];
	_trans = _EDN_keyword_index_offsets[cs];

	_klen = _EDN_keyword_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _EDN_keyword_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _EDN_keyword_indicies[_trans];
	cs = _EDN_keyword_trans_targs[_trans];

	if ( _EDN_keyword_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _EDN_keyword_actions + _EDN_keyword_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 131 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 417 "edn_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 145 "edn_parser.rl"

    if (cs >= EDN_keyword_first_final) {
        std::string buf;
        buf.append(p_save, p - p_save);
        o = Rice::Symbol(buf);
        return p + 1;
    }

    return NULL;
}



// ============================================================
// string parsing
//

#line 448 "edn_parser.cc"
static const char _EDN_string_actions[] = {
	0, 2, 0, 1
};

static const char _EDN_string_key_offsets[] = {
	0, 0, 1, 5, 8, 14, 20, 26,
	32
};

static const char _EDN_string_trans_keys[] = {
	34, 34, 92, 0, 31, 117, 0, 31,
	48, 57, 65, 70, 97, 102, 48, 57,
	65, 70, 97, 102, 48, 57, 65, 70,
	97, 102, 48, 57, 65, 70, 97, 102,
	0
};

static const char _EDN_string_single_lengths[] = {
	0, 1, 2, 1, 0, 0, 0, 0,
	0
};

static const char _EDN_string_range_lengths[] = {
	0, 0, 1, 1, 3, 3, 3, 3,
	0
};

static const char _EDN_string_index_offsets[] = {
	0, 0, 2, 6, 9, 13, 17, 21,
	25
};

static const char _EDN_string_indicies[] = {
	0, 1, 2, 3, 1, 0, 4, 1,
	0, 5, 5, 5, 1, 6, 6, 6,
	1, 7, 7, 7, 1, 0, 0, 0,
	1, 1, 0
};

static const char _EDN_string_trans_targs[] = {
	2, 0, 8, 3, 4, 5, 6, 7
};

static const char _EDN_string_trans_actions[] = {
	0, 0, 1, 0, 0, 0, 0, 0
};

static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;

static const int EDN_string_en_main = 1;


#line 179 "edn_parser.rl"



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

#line 534 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 210 "edn_parser.rl"
    p_save = p;

#line 542 "edn_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _EDN_string_trans_keys + _EDN_string_key_offsets[cs];
	_trans = _EDN_string_index_offsets[cs];

	_klen = _EDN_string_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _EDN_string_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _EDN_string_indicies[_trans];
	cs = _EDN_string_trans_targs[_trans];

	if ( _EDN_string_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _EDN_string_actions + _EDN_string_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 167 "edn_parser.rl"
	{
        if (!EDN_parse_byte_stream(p_save + 1, p, s)) {
            p--;
            {p++; goto _out; }
        } else {
            {p = (( p + 1))-1;}
        }
    }
	break;
	case 1:
#line 176 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 631 "edn_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 212 "edn_parser.rl"

    if (cs >= EDN_string_first_final) {
        o = s;
        return p + 1;
    }
    return NULL;
}

// ============================================================
// decimal parsing grammar
//

#line 657 "edn_parser.cc"
static const char _EDN_decimal_actions[] = {
	0, 1, 0
};

static const char _EDN_decimal_key_offsets[] = {
	0, 0, 4, 7, 10, 12, 16, 18,
	23, 29, 29
};

static const char _EDN_decimal_trans_keys[] = {
	45, 48, 49, 57, 48, 49, 57, 46,
	69, 101, 48, 57, 43, 45, 48, 57,
	48, 57, 46, 69, 101, 48, 57, 69,
	101, 45, 46, 48, 57, 69, 101, 45,
	46, 48, 57, 0
};

static const char _EDN_decimal_single_lengths[] = {
	0, 2, 1, 3, 0, 2, 0, 3,
	2, 0, 2
};

static const char _EDN_decimal_range_lengths[] = {
	0, 1, 1, 0, 1, 1, 1, 1,
	2, 0, 2
};

static const char _EDN_decimal_index_offsets[] = {
	0, 0, 4, 7, 11, 13, 17, 19,
	24, 29, 30
};

static const char _EDN_decimal_indicies[] = {
	0, 2, 3, 1, 2, 3, 1, 4,
	5, 5, 1, 6, 1, 7, 7, 8,
	1, 8, 1, 4, 5, 5, 3, 1,
	5, 5, 1, 6, 9, 1, 1, 1,
	1, 8, 9, 0
};

static const char _EDN_decimal_trans_targs[] = {
	2, 0, 3, 7, 4, 5, 8, 6,
	10, 9
};

static const char _EDN_decimal_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1
};

static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 8;

static const int EDN_decimal_en_main = 1;


#line 235 "edn_parser.rl"



const char* edn::Parser::EDN_parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;


#line 724 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 244 "edn_parser.rl"
    p_save = p;

#line 732 "edn_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _EDN_decimal_trans_keys + _EDN_decimal_key_offsets[cs];
	_trans = _EDN_decimal_index_offsets[cs];

	_klen = _EDN_decimal_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _EDN_decimal_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _EDN_decimal_indicies[_trans];
	cs = _EDN_decimal_trans_targs[_trans];

	if ( _EDN_decimal_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _EDN_decimal_actions + _EDN_decimal_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 229 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 810 "edn_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 246 "edn_parser.rl"

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

#line 839 "edn_parser.cc"
static const char _EDN_integer_actions[] = {
	0, 1, 0
};

static const char _EDN_integer_key_offsets[] = {
	0, 0, 4, 7, 9, 9
};

static const char _EDN_integer_trans_keys[] = {
	45, 48, 49, 57, 48, 49, 57, 48,
	57, 48, 57, 0
};

static const char _EDN_integer_single_lengths[] = {
	0, 2, 1, 0, 0, 0
};

static const char _EDN_integer_range_lengths[] = {
	0, 1, 1, 1, 0, 1
};

static const char _EDN_integer_index_offsets[] = {
	0, 0, 4, 7, 9, 10
};

static const char _EDN_integer_indicies[] = {
	0, 2, 3, 1, 2, 3, 1, 1,
	4, 1, 3, 4, 0
};

static const char _EDN_integer_trans_targs[] = {
	2, 0, 3, 5, 4
};

static const char _EDN_integer_trans_actions[] = {
	0, 0, 0, 0, 1
};

static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 268 "edn_parser.rl"


const char* edn::Parser::EDN_parse_integer(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;


#line 893 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 276 "edn_parser.rl"
    p_save = p;

#line 901 "edn_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _EDN_integer_trans_keys + _EDN_integer_key_offsets[cs];
	_trans = _EDN_integer_index_offsets[cs];

	_klen = _EDN_integer_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _EDN_integer_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _EDN_integer_indicies[_trans];
	cs = _EDN_integer_trans_targs[_trans];

	if ( _EDN_integer_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _EDN_integer_actions + _EDN_integer_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 265 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 979 "edn_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 278 "edn_parser.rl"

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

#line 1008 "edn_parser.cc"
static const char _EDN_vector_actions[] = {
	0, 1, 0, 1, 1
};

static const char _EDN_vector_key_offsets[] = {
	0, 0, 1, 18, 19
};

static const char _EDN_vector_trans_keys[] = {
	91, 13, 32, 44, 45, 59, 93, 102,
	110, 116, 9, 10, 34, 35, 48, 58,
	91, 92, 10, 0
};

static const char _EDN_vector_single_lengths[] = {
	0, 1, 9, 1, 0
};

static const char _EDN_vector_range_lengths[] = {
	0, 0, 4, 0, 0
};

static const char _EDN_vector_index_offsets[] = {
	0, 0, 2, 16, 18
};

static const char _EDN_vector_indicies[] = {
	0, 1, 0, 0, 0, 2, 3, 4,
	2, 2, 2, 0, 2, 2, 2, 1,
	0, 3, 1, 0
};

static const char _EDN_vector_trans_targs[] = {
	2, 0, 2, 3, 4
};

static const char _EDN_vector_trans_actions[] = {
	0, 0, 1, 0, 3
};

static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;

static const int EDN_vector_en_main = 1;


#line 317 "edn_parser.rl"


//
//
//
const char* edn::Parser::EDN_parse_vector(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;
    Rice::Array arr;


#line 1068 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 329 "edn_parser.rl"

#line 1075 "edn_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _EDN_vector_trans_keys + _EDN_vector_key_offsets[cs];
	_trans = _EDN_vector_index_offsets[cs];

	_klen = _EDN_vector_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _EDN_vector_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _EDN_vector_indicies[_trans];
	cs = _EDN_vector_trans_targs[_trans];

	if ( _EDN_vector_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _EDN_vector_actions + _EDN_vector_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 298 "edn_parser.rl"
	{
        Rice::Object v;
        const char *np = EDN_parse_value(p, pe, v);
        if (np == NULL) {
            p--; {p++; goto _out; }
        } else {
            arr.push(v);
            {p = (( np))-1;}
        }
    }
	break;
	case 1:
#line 309 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 1166 "edn_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 330 "edn_parser.rl"

    if (cs >= EDN_vector_first_final) {
        o = arr;
        return p + 1;
    }

    return NULL;
}


// ============================================================
// main parsing machine
//

#line 1194 "edn_parser.cc"
static const char _EDN_actions[] = {
	0, 1, 0
};

static const char _EDN_key_offsets[] = {
	0, 0, 7, 8, 9
};

static const char _EDN_trans_keys[] = {
	13, 32, 44, 59, 91, 9, 10, 10,
	10, 13, 32, 44, 59, 9, 10, 0
};

static const char _EDN_single_lengths[] = {
	0, 5, 1, 1, 4
};

static const char _EDN_range_lengths[] = {
	0, 1, 0, 0, 1
};

static const char _EDN_index_offsets[] = {
	0, 0, 7, 9, 11
};

static const char _EDN_indicies[] = {
	0, 0, 0, 2, 3, 0, 1, 0,
	2, 5, 4, 5, 5, 5, 4, 5,
	1, 0
};

static const char _EDN_trans_targs[] = {
	1, 0, 2, 4, 3, 4
};

static const char _EDN_trans_actions[] = {
	0, 0, 0, 1, 0, 0
};

static const int EDN_start = 1;
static const int EDN_error = 0;

static const int EDN_en_main = 1;


#line 362 "edn_parser.rl"


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


#line 1255 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 376 "edn_parser.rl"
    p = &buf[0];
    pe = buf + len;
    eof = pe;

#line 1265 "edn_parser.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _EDN_trans_keys + _EDN_key_offsets[cs];
	_trans = _EDN_index_offsets[cs];

	_klen = _EDN_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _EDN_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _EDN_indicies[_trans];
	cs = _EDN_trans_targs[_trans];

	if ( _EDN_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _EDN_actions + _EDN_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 348 "edn_parser.rl"
	{
        const char* np = EDN_parse_vector(p, pe, result);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
#line 1346 "edn_parser.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 380 "edn_parser.rl"

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
