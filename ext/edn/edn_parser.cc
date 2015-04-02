
#line 1 "edn_parser.rl"
#include <iostream>
#include <string>
#include <fstream>

#include <rice/Hash.hpp>
#include <rice/Array.hpp>
#include <rice/to_from_ruby.hpp>

#include "edn_parser.h"

using namespace std;


Rice::Object edn::Parser::parse(const std::string& file) {
    Rice::Object rslt = Qnil;

    ifstream f(file);

    if (f.is_open()) {

        f.seekg (0, f.end);
        long len = f.tellg();
        f.seekg (0, f.beg);

        char* buf = new char[len];

        f.read(buf, len);
        f.close();

        rslt = execute(buf, len);

        delete [] buf;
    }
    return rslt;
}

// based on https://github.com/edn-format/edn


#line 59 "edn_parser.rl"



#line 47 "edn_parser.cc"
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
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 101 "edn_parser.rl"



const char *edn::Parser::EDN_parse_value(const char *p, const char *pe, Rice::Object& result)
{
    cerr << "+ == " << __FUNCTION__ << " == +" << endl;

    int cs;


#line 116 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 111 "edn_parser.rl"

#line 123 "edn_parser.cc"
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
#line 92 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 144 "edn_parser.cc"
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
#line 67 "edn_parser.rl"
	{
        result = Qnil;
    }
	break;
	case 1:
#line 70 "edn_parser.rl"
	{
        result = Qfalse;
    }
	break;
	case 2:
#line 73 "edn_parser.rl"
	{
        result = Qtrue;
    }
	break;
	case 3:
#line 77 "edn_parser.rl"
	{
        const char *np;

        np = EDN_parse_decimal(p, pe, result);
        if (np != NULL) {p = (( np))-1;}
        np = EDN_parse_integer(p, pe, result);
        if (np != NULL) {p = (( np))-1;}
        p--; {p++; goto _out; }
    }
	break;
	case 4:
#line 87 "edn_parser.rl"
	{
        const char *np = EDN_parse_vector(p, pe, result);
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

#line 112 "edn_parser.rl"

    if (cs >= EDN_value_first_final) {
        return p;
    }

    return NULL;
}




#line 270 "edn_parser.cc"
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
static const int EDN_decimal_error = 0;

static const int EDN_decimal_en_main = 1;


#line 134 "edn_parser.rl"



const char* edn::Parser::EDN_parse_decimal(const char *p, const char *pe, Rice::Object& o)
{
    int cs;


#line 337 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 142 "edn_parser.rl"
    p_save = p;

#line 345 "edn_parser.cc"
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
#line 128 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 423 "edn_parser.cc"
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

#line 144 "edn_parser.rl"

    if (cs >= EDN_decimal_first_final) {
        long len = p - p_save;
        buf = "";
        buf.append(p_save, len);
        double value;
        istringstream(buf) >> value;
        o = to_ruby<double>(value);
        return p + 1;
    }

    return NULL;
}



#line 453 "edn_parser.cc"
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
static const int EDN_integer_error = 0;

static const int EDN_integer_en_main = 1;


#line 167 "edn_parser.rl"


const char* edn::Parser::EDN_parse_integer(const char *p, const char *pe, Rice::Object& r_int)
{
    cerr << "+ == " << __FUNCTION__ << " == +" << endl;
    int cs;


#line 508 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 175 "edn_parser.rl"
    p_save = p;
    //    json->memo = p;

#line 517 "edn_parser.cc"
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
#line 164 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 595 "edn_parser.cc"
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

#line 178 "edn_parser.rl"

    if (cs >= EDN_integer_first_final) {
        long len = p - p_save;
        buf = "";
        buf.append(p_save, len);
        int value;
        istringstream(buf) >> value;
        r_int = to_ruby<int>(value);
        return p + 1;
    }

    return NULL;
}




#line 626 "edn_parser.cc"
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
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 219 "edn_parser.rl"



const char* edn::Parser::EDN_parse_vector(const char *p, const char *pe, Rice::Object& result)
{
    cerr << "+ == " << __FUNCTION__ << " == +" << endl;
    int cs;
    Rice::Array arr;


#line 685 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 229 "edn_parser.rl"

#line 692 "edn_parser.cc"
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
#line 200 "edn_parser.rl"
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
#line 211 "edn_parser.rl"
	{ p--; {p++; goto _out; } }
	break;
#line 783 "edn_parser.cc"
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

#line 230 "edn_parser.rl"

    if(cs >= EDN_vector_first_final) {
        result = arr;
        return p + 1;
    }

    cerr << "Parse error: unexpected token at '" << p << "'" << endl;
    return NULL;
}


#line 808 "edn_parser.cc"
static const char _EDN_actions[] = {
	0, 1, 1, 2, 0, 1
};

static const char _EDN_key_offsets[] = {
	0, 0, 15
};

static const char _EDN_trans_keys[] = {
	13, 32, 44, 45, 91, 92, 102, 110,
	116, 9, 10, 34, 35, 48, 57, 13,
	32, 44, 9, 10, 0
};

static const char _EDN_single_lengths[] = {
	0, 9, 3
};

static const char _EDN_range_lengths[] = {
	0, 3, 1
};

static const char _EDN_index_offsets[] = {
	0, 0, 13
};

static const char _EDN_indicies[] = {
	0, 0, 0, 2, 3, 2, 2, 2,
	2, 0, 2, 2, 1, 4, 4, 4,
	4, 1, 0
};

static const char _EDN_trans_targs[] = {
	1, 0, 2, 2, 2
};

static const char _EDN_trans_actions[] = {
	0, 0, 1, 3, 0
};

static const int EDN_start = 1;
static const int EDN_first_final = 2;
static const int EDN_error = 0;

static const int EDN_en_main = 1;


#line 265 "edn_parser.rl"



Rice::Object edn::Parser::execute(const char* buf, long len)
{
    cerr << "+ == " << __FUNCTION__ << " == +" << endl;

    int cs;
    const char *p;
    const char *pe;
    const char *eof;
    Rice::Object result;


#line 871 "edn_parser.cc"
	{
	cs = EDN_start;
	}

#line 279 "edn_parser.rl"
    p = &buf[0];
    pe = buf + len;
    eof = pe;

#line 881 "edn_parser.cc"
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
#line 245 "edn_parser.rl"
	{
        const char* np = EDN_parse_vector(p, pe, result);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
	case 1:
#line 255 "edn_parser.rl"
	{
        const char *np = EDN_parse_value(p, pe, result);
        if (np == NULL) { p--; {p++; goto _out; } } else {p = (( np))-1;}
    }
	break;
#line 969 "edn_parser.cc"
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

#line 283 "edn_parser.rl"

    if ( cs == EDN_error )
        return Qnil;

    return result;
}

/*
 * Local variables:
 * mode: c
 * c-file-style: ruby
 * indent-tabs-mode: nil
 * End:
 */
