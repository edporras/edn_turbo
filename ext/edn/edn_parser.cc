
#line 1 "edn_parser.rl"
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


#line 34 "edn_parser.rl"



#line 22 "edn_parser.cc"
static const char _EDN_value_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1,
	3, 1, 4, 1, 5
};

static const char _EDN_value_key_offsets[] = {
	0, 0, 7, 8, 9, 10, 11, 12,
	13, 14, 15, 16
};

static const char _EDN_value_trans_keys[] = {
	45, 91, 102, 110, 116, 48, 57, 97,
	108, 115, 101, 105, 108, 114, 117, 101,
	0
};

static const char _EDN_value_single_lengths[] = {
	0, 5, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 0
};

static const char _EDN_value_range_lengths[] = {
	0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0
};

static const char _EDN_value_index_offsets[] = {
	0, 0, 7, 9, 11, 13, 15, 17,
	19, 21, 23, 25
};

static const char _EDN_value_trans_targs[] = {
	11, 11, 2, 6, 8, 11, 0, 3,
	0, 4, 0, 5, 0, 11, 0, 7,
	0, 11, 0, 9, 0, 10, 0, 11,
	0, 0, 0
};

static const char _EDN_value_trans_actions[] = {
	7, 9, 0, 0, 0, 7, 0, 0,
	0, 0, 0, 0, 0, 3, 0, 0,
	0, 1, 0, 0, 0, 0, 0, 5,
	0, 0, 0
};

static const char _EDN_value_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 11
};

static const int EDN_value_start = 1;
static const int EDN_value_first_final = 11;

static const int EDN_value_en_main = 1;


#line 79 "edn_parser.rl"



const char *edn::Parser::EDN_parse_value(const char *p, const char *pe, Rice::Object& result)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;

    int cs;


#line 90 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 89 "edn_parser.rl"

#line 97 "edn_parser.cc"
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
	case 5:
#line 70 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 118 "edn_parser.cc"
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
#line 42 "edn_parser.rl"
	{
        result = Qnil;
    }
	break;
	case 1:
#line 45 "edn_parser.rl"
	{
        result = Qfalse;
    }
	break;
	case 2:
#line 48 "edn_parser.rl"
	{
        result = Qtrue;
    }
	break;
	case 3:
#line 52 "edn_parser.rl"
	{
        const char *np;

        np = EDN_parse_decimal(p, pe, result);
        if (np != NULL) {
            {p = (( np))-1;}
        } else {
            np = EDN_parse_integer(p, pe, result);
            if (np != NULL) {p = (( np))-1;}
        }
        p--; {p++; goto _out; }
    }
	break;
	case 4:
#line 65 "edn_parser.rl"
	{
        const char *np = EDN_parse_vector(p, pe, result);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
#line 222 "edn_parser.cc"
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

#line 90 "edn_parser.rl"

    if (cs >= EDN_value_first_final) {
        return p;
    }

    return NULL;
}




#line 247 "edn_parser.cc"
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


#line 112 "edn_parser.rl"



const char* edn::Parser::EDN_parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;


#line 314 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 121 "edn_parser.rl"
    p_save = p;

#line 322 "edn_parser.cc"
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
#line 106 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 400 "edn_parser.cc"
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

#line 123 "edn_parser.rl"

    if (cs >= EDN_decimal_first_final) {
        long len = p - p_save;
        std::string buf;
        buf.append(p_save, len);
        double value;
        std::istringstream(buf) >> value;
        o = to_ruby<double>(value);
        return p + 1;
    }

    return NULL;
}



#line 430 "edn_parser.cc"
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


#line 146 "edn_parser.rl"


const char* edn::Parser::EDN_parse_integer(const char *p, const char *pe, Rice::Object& r_int)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;


#line 484 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 154 "edn_parser.rl"
    p_save = p;

#line 492 "edn_parser.cc"
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
#line 143 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 570 "edn_parser.cc"
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

#line 156 "edn_parser.rl"

    if (cs >= EDN_integer_first_final) {
        long len = p - p_save;
        std::string buf;
        buf.append(p_save, len);
        int value;
        std::istringstream(buf) >> value;
        r_int = to_ruby<int>(value);
        return p + 1;
    }

    return NULL;
}



// ============================================================
// vector parsing machine
//

#line 604 "edn_parser.cc"
static const char _EDN_vector_actions[] = {
	0, 1, 0, 1, 1
};

static const char _EDN_vector_key_offsets[] = {
	0, 0, 1, 17
};

static const char _EDN_vector_trans_keys[] = {
	91, 13, 32, 44, 45, 93, 102, 110,
	116, 9, 10, 34, 35, 48, 57, 91,
	92, 0
};

static const char _EDN_vector_single_lengths[] = {
	0, 1, 8, 0
};

static const char _EDN_vector_range_lengths[] = {
	0, 0, 4, 0
};

static const char _EDN_vector_index_offsets[] = {
	0, 0, 2, 15
};

static const char _EDN_vector_indicies[] = {
	0, 1, 0, 0, 0, 2, 3, 2,
	2, 2, 0, 2, 2, 2, 1, 1,
	0
};

static const char _EDN_vector_trans_targs[] = {
	2, 0, 2, 3
};

static const char _EDN_vector_trans_actions[] = {
	0, 0, 1, 3
};

static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 3;

static const int EDN_vector_en_main = 1;


#line 200 "edn_parser.rl"


//
//
//
const char* edn::Parser::EDN_parse_vector(const char *p, const char *pe, Rice::Object& result)
{
    std::cerr << "+ == " << __FUNCTION__ << " == +" << std::endl;
    int cs;
    Rice::Array arr;


#line 664 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 212 "edn_parser.rl"

#line 671 "edn_parser.cc"
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
#line 181 "edn_parser.rl"
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
#line 192 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 762 "edn_parser.cc"
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

#line 213 "edn_parser.rl"

    if (cs >= EDN_vector_first_final) {
        result = arr;
        return p + 1;
    }

    return NULL;
}


// ============================================================
// main parsing machine
//

#line 790 "edn_parser.cc"
static const char _EDN_actions[] = {
	0, 1, 0
};

static const char _EDN_key_offsets[] = {
	0, 0, 6
};

static const char _EDN_trans_keys[] = {
	13, 32, 44, 91, 9, 10, 13, 32,
	44, 9, 10, 0
};

static const char _EDN_single_lengths[] = {
	0, 4, 3
};

static const char _EDN_range_lengths[] = {
	0, 1, 1
};

static const char _EDN_index_offsets[] = {
	0, 0, 6
};

static const char _EDN_indicies[] = {
	0, 0, 0, 2, 0, 1, 3, 3,
	3, 3, 1, 0
};

static const char _EDN_trans_targs[] = {
	1, 0, 2, 2
};

static const char _EDN_trans_actions[] = {
	0, 0, 1, 0
};

static const int EDN_start = 1;
static const int EDN_error = 0;

static const int EDN_en_main = 1;


#line 245 "edn_parser.rl"


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


#line 850 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 259 "edn_parser.rl"
    p = &buf[0];
    pe = buf + len;
    eof = pe;

#line 860 "edn_parser.cc"
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
#line 231 "edn_parser.rl"
	{
        const char* np = EDN_parse_vector(p, pe, result);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
#line 941 "edn_parser.cc"
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

#line 263 "edn_parser.rl"

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
