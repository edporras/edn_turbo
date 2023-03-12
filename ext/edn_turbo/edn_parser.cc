
#line 1 "edn_parser.rl"
// The MIT License (MIT)

// Copyright (c) 2015-2023 Ed Porras

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <cstring>

#include <ruby/ruby.h>

#include "util.h"
#include "parser.h"

//
// EDN spec at: https://github.com/edn-format/edn
//
//
// many thanks to Florian Frank for json-ruby which was essential in
// helping me learn about ragel
//


#line 81 "edn_parser.rl"


// ============================================================
// machine for parsing various EDN token types
//


#line 55 "edn_parser.cc"
static const int EDN_value_start = 1;
static const int EDN_value_first_final = 2;
static const int EDN_value_error = 0;

static const int EDN_value_en_main = 1;


#line 202 "edn_parser.rl"



const char *edn::Parser::parse_value(const char *p, const char *pe, VALUE& v)
{
   //    std::cerr << __FUNCTION__ << "() p: \"" << p << "\"" << std::endl;
   int cs;


#line 73 "edn_parser.cc"
	{
	cs = EDN_value_start;
	}

#line 211 "edn_parser.rl"

#line 80 "edn_parser.cc"
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
#line 130 "edn_parser.rl"
	{
      // stand-alone operators *, +, -, etc.
      const char *np = parse_operator(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr2:
#line 93 "edn_parser.rl"
	{
      // string types within double-quotes
      const char *np = parse_string(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr3:
#line 182 "edn_parser.rl"
	{
      // handles tokens w/ leading # ("#_", "#{", and tagged elems)
      const char *np = parse_dispatch(p + 1, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr4:
#line 164 "edn_parser.rl"
	{
      // (
      const char *np = parse_list(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr5:
#line 105 "edn_parser.rl"
	{
      // tokens w/ leading digits: non-negative integers & decimals.
      // try to parse a decimal first
      const char *np = parse_decimal(p, pe, v);
      if (np == nullptr) {
         // if we can't, try to parse it as a ratio
         np = parse_ratio(p, pe, v);

         // otherwise, an int
         if (np == nullptr) {
            np = parse_integer(p, pe, v);
         }
      }

      if (np) {
         {p = (( np))-1;}
         p--;
         {p++; cs = 2; goto _out;}
      }
      else {
         error(__FUNCTION__, "number format error", *p);
         {p = (( pe))-1;}
      }
   }
	goto st2;
tr6:
#line 99 "edn_parser.rl"
	{
      // tokens with a leading ':'
      const char *np = parse_keyword(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr7:
#line 142 "edn_parser.rl"
	{
      // user identifiers and reserved keywords (true, false, nil)
      VALUE sym = Qnil;
      const char *np = parse_symbol(p, pe, sym);
      if (np == nullptr) { {p = (( pe))-1;} } else {
         // parse_symbol will make 'sym' a ruby string
         if      (std::strcmp(RSTRING_PTR(sym), "true") == 0)  { v = Qtrue; }
         else if (std::strcmp(RSTRING_PTR(sym), "false") == 0) { v = Qfalse; }
         else if (std::strcmp(RSTRING_PTR(sym), "nil") == 0)   { v = Qnil; }
         else {
            v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
         }
         {p = (( np))-1;}
      }
   }
	goto st2;
tr8:
#line 158 "edn_parser.rl"
	{
      // [
      const char *np = parse_vector(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr9:
#line 136 "edn_parser.rl"
	{
      // tokens w/ leading \ (escaped characters \newline, \c, etc.)
      const char *np = parse_esc_char(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr10:
#line 176 "edn_parser.rl"
	{
      // ^
      const char *np = parse_meta(p, pe);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
tr11:
#line 170 "edn_parser.rl"
	{
      // {
      const char *np = parse_map(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
#line 245 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 212 "edn_parser.rl"

   if (cs >= EDN_value_first_final) {
      return p;
   }
   else if (cs == EDN_value_error) {
      error(__FUNCTION__, "token error", *p);
      return pe;
   }
   else if (cs == EDN_value_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// string parsing - incoming string is raw so interpreting utf
// encodings & unicode values might be necessary. To optimize things a
// bit, we mark the string for encoding if anything outside of the
// ascii range is found.
//

#line 276 "edn_parser.cc"
static const int EDN_string_start = 1;
static const int EDN_string_first_final = 8;
static const int EDN_string_error = 0;

static const int EDN_string_en_main = 1;


#line 257 "edn_parser.rl"



const char* edn::Parser::parse_string(const char *p, const char *pe, VALUE& v)
{
   static const char* EDN_TYPE = "string";
   int cs;
   bool encode = false;


#line 295 "edn_parser.cc"
	{
	cs = EDN_string_start;
	}

#line 267 "edn_parser.rl"
   const char* p_save = p;

#line 303 "edn_parser.cc"
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
tr2:
#line 246 "edn_parser.rl"
	{
      encode = true;
   }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 326 "edn_parser.cc"
	switch( (*p) ) {
		case 34: goto tr3;
		case 92: goto tr4;
	}
	if ( -62 <= (*p) && (*p) <= -11 )
		goto tr2;
	goto st2;
tr3:
#line 238 "edn_parser.rl"
	{
      if (edn::util::parse_byte_stream(p_save + 1, p, v, encode)) {
         {p = (( p + 1))-1;}
      } else {
         p--; {p++; cs = 8; goto _out;}
      }
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 8; goto _out;} }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 350 "edn_parser.cc"
	goto st0;
tr4:
#line 246 "edn_parser.rl"
	{
      encode = true;
   }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 362 "edn_parser.cc"
	switch( (*p) ) {
		case 34: goto tr2;
		case 47: goto tr2;
		case 92: goto tr2;
		case 98: goto tr2;
		case 102: goto tr2;
		case 110: goto tr2;
		case 114: goto tr2;
		case 116: goto tr2;
		case 117: goto tr5;
	}
	goto st2;
tr5:
#line 246 "edn_parser.rl"
	{
      encode = true;
   }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 385 "edn_parser.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto tr6;
	} else
		goto tr6;
	goto st0;
tr6:
#line 246 "edn_parser.rl"
	{
      encode = true;
   }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 405 "edn_parser.cc"
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
#line 246 "edn_parser.rl"
	{
      encode = true;
   }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 425 "edn_parser.cc"
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
#line 246 "edn_parser.rl"
	{
      encode = true;
   }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 445 "edn_parser.cc"
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
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	break;
#line 477 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 269 "edn_parser.rl"

   if (cs >= EDN_string_first_final) {
      return p + 1;
   }
   else if (cs == EDN_string_error) {
      return pe;
   }
   else if (cs == EDN_string_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// keyword parsing
//

#line 502 "edn_parser.cc"
static const int EDN_keyword_start = 1;
static const int EDN_keyword_first_final = 3;
static const int EDN_keyword_error = 0;

static const int EDN_keyword_en_main = 1;


#line 299 "edn_parser.rl"



const char* edn::Parser::parse_keyword(const char *p, const char *pe, VALUE& v)
{
   int cs;


#line 519 "edn_parser.cc"
	{
	cs = EDN_keyword_start;
	}

#line 307 "edn_parser.rl"
   const char* p_save = p;

#line 527 "edn_parser.cc"
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
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 601 "edn_parser.cc"
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

#line 309 "edn_parser.rl"

   if (cs >= EDN_keyword_first_final) {
      std::string buf;
      uintmax_t len = p - p_save;
      // don't include leading ':' because the ruby symbol will handle it
      buf.append(p_save + 1, len - 1);
      v = ID2SYM(rb_intern(buf.c_str()));
      return p;
   }
   else if (cs == EDN_keyword_error) {
      error(__FUNCTION__, "invalid keyword", *p);
      return pe;
   }
   else if (cs == EDN_keyword_en_main) {} // silence ragel warning
   return nullptr;
}


// ============================================================
// decimal parsing machine
//

#line 663 "edn_parser.cc"
static const int EDN_decimal_start = 1;
static const int EDN_decimal_first_final = 9;

static const int EDN_decimal_en_main = 1;


#line 341 "edn_parser.rl"



const char* edn::Parser::parse_decimal(const char *p, const char *pe, VALUE& v)
{
   int cs;


#line 679 "edn_parser.cc"
	{
	cs = EDN_decimal_start;
	}

#line 349 "edn_parser.rl"
   const char* p_save = p;

#line 687 "edn_parser.cc"
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
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 10; goto _out;} }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 748 "edn_parser.cc"
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

#line 351 "edn_parser.rl"

   if (cs >= EDN_decimal_first_final) {
      v = edn::util::float_to_ruby(p_save, p - p_save);
      return p + 1;
   }
   else if (cs == EDN_decimal_en_main) {} // silence ragel warning
   return nullptr;
}


// ============================================================
// integer parsing machine - M suffix will return a BigNum
//

#line 879 "edn_parser.cc"
static const int EDN_integer_start = 1;
static const int EDN_integer_first_final = 3;

static const int EDN_integer_en_main = 1;


#line 374 "edn_parser.rl"


const char* edn::Parser::parse_integer(const char *p, const char *pe, VALUE& v)
{
   int cs;


#line 894 "edn_parser.cc"
	{
	cs = EDN_integer_start;
	}

#line 381 "edn_parser.rl"
   const char* p_save = p;

#line 902 "edn_parser.cc"
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
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 951 "edn_parser.cc"
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

#line 383 "edn_parser.rl"

   if (cs >= EDN_integer_first_final) {
      v = edn::util::integer_to_ruby(p_save, p - p_save);
      return p + 1;
   }
   else if (cs == EDN_integer_en_main) {} // silence ragel warning
   return nullptr;
}


// ============================================================
// ratio parsing machine
//

#line 1007 "edn_parser.cc"
static const int EDN_ratio_start = 1;
static const int EDN_ratio_first_final = 6;

static const int EDN_ratio_en_main = 1;


#line 406 "edn_parser.rl"



const char* edn::Parser::parse_ratio(const char *p, const char *pe, VALUE& v)
{
   int cs;


#line 1023 "edn_parser.cc"
	{
	cs = EDN_ratio_start;
	}

#line 414 "edn_parser.rl"
   const char* p_save = p;

#line 1031 "edn_parser.cc"
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
	if ( (*p) == 47 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 48 )
		goto st6;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 43: goto st0;
		case 45: goto st0;
	}
	if ( 47 <= (*p) && (*p) <= 57 )
		goto st0;
	goto tr7;
tr7:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1093 "edn_parser.cc"
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 43: goto st0;
		case 45: goto st0;
		case 47: goto st0;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto tr7;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 47 )
		goto st4;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st5;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 416 "edn_parser.rl"

   if (cs >= EDN_ratio_first_final) {
      v = edn::util::ratio_to_ruby(p_save, p - p_save);
      return p + 1;
   }
   else if (cs == EDN_ratio_en_main) {} // silence ragel warning
   return nullptr;
}

// ============================================================
// operator parsing - handles tokens w/ a leading operator:
//
// 1. symbols w/ leading operator: -something, .somethingelse
// 2. number values w/ leading - or +
// 3. stand-alone operators: +, -, /, *, etc.
//

#line 1147 "edn_parser.cc"
static const int EDN_operator_start = 1;
static const int EDN_operator_first_final = 3;
static const int EDN_operator_error = 0;

static const int EDN_operator_en_main = 1;


#line 493 "edn_parser.rl"



const char* edn::Parser::parse_operator(const char *p, const char *pe, VALUE& v)
{
   int cs;


#line 1164 "edn_parser.cc"
	{
	cs = EDN_operator_start;
	}

#line 501 "edn_parser.rl"
   const char* p_save = p;

#line 1172 "edn_parser.cc"
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
		case 46: goto st11;
		case 47: goto st3;
		case 95: goto st3;
	}
	if ( (*p) > 39 ) {
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
		case 33: goto tr9;
		case 44: goto tr7;
		case 59: goto tr10;
		case 95: goto tr9;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto tr9;
		} else if ( (*p) >= 9 )
			goto tr7;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr9;
		} else if ( (*p) >= 65 )
			goto tr9;
	} else
		goto tr9;
	goto tr6;
tr6:
#line 477 "edn_parser.rl"
	{
      // stand-alone operators (-, +, /, ... etc)
      char op[2] = { *p_save, 0 };
      VALUE sym = rb_str_new2(op);
      v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
tr11:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
tr17:
#line 438 "edn_parser.rl"
	{
      // parse a symbol including the leading operator (-, +, .)
      VALUE sym = Qnil;
      const char *np = parse_symbol(p_save, pe, sym);
      if (np == nullptr) { {p = (( pe))-1;} } else {
         if (sym != Qnil)
            v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
         {p = (( np))-1;}
      }
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 1258 "edn_parser.cc"
	goto st0;
tr5:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st5;
tr7:
#line 477 "edn_parser.rl"
	{
      // stand-alone operators (-, +, /, ... etc)
      char op[2] = { *p_save, 0 };
      VALUE sym = rb_str_new2(op);
      v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr8:
#line 477 "edn_parser.rl"
	{
      // stand-alone operators (-, +, /, ... etc)
      char op[2] = { *p_save, 0 };
      VALUE sym = rb_str_new2(op);
      v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
   }
#line 47 "edn_parser.rl"
	{ line_number++; }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr12:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
tr13:
#line 47 "edn_parser.rl"
	{ line_number++; }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 1302 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr13;
		case 32: goto tr12;
		case 33: goto st0;
		case 44: goto tr12;
		case 59: goto tr14;
		case 95: goto st0;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st0;
		} else if ( (*p) >= 9 )
			goto tr12;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st0;
		} else if ( (*p) >= 65 )
			goto st0;
	} else
		goto st0;
	goto tr11;
tr10:
#line 477 "edn_parser.rl"
	{
      // stand-alone operators (-, +, /, ... etc)
      char op[2] = { *p_save, 0 };
      VALUE sym = rb_str_new2(op);
      v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr14:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 1345 "edn_parser.cc"
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
tr9:
#line 438 "edn_parser.rl"
	{
      // parse a symbol including the leading operator (-, +, .)
      VALUE sym = Qnil;
      const char *np = parse_symbol(p_save, pe, sym);
      if (np == nullptr) { {p = (( pe))-1;} } else {
         if (sym != Qnil)
            v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
         {p = (( np))-1;}
      }
   }
	goto st7;
tr16:
#line 449 "edn_parser.rl"
	{
      // parse a number with the leading symbol - this is slightly
      // different than the one within EDN_value since it includes
      // the leading - or +
      //
      // try to parse a decimal first
      const char *np = parse_decimal(p_save, pe, v);
      if (np == nullptr) {
         // if we can't, try to parse it as a ratio
         np = parse_ratio(p_save, pe, v);

         if (np == nullptr) {
            // again, if we can't, try to parse it as an int
            np = parse_integer(p_save, pe, v);
         }
      }

      if (np) {
         {p = (( np))-1;}
         p--;
         {p++; cs = 7; goto _out;}
      }
      else {
         error(__FUNCTION__, "number format error", *p);
         {p = (( pe))-1;}
      }
   }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 1403 "edn_parser.cc"
	switch( (*p) ) {
		case 33: goto st0;
		case 95: goto st0;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st0;
		} else if ( (*p) >= 35 )
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
	goto tr11;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st9;
		case 44: goto tr7;
		case 59: goto tr10;
		case 95: goto st9;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 35 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr7;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 47 )
				goto st9;
		} else
			goto st9;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 63 )
				goto st9;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st9;
		} else
			goto st9;
	} else
		goto tr16;
	goto tr6;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 33: goto tr18;
		case 95: goto tr18;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr18;
		} else if ( (*p) >= 35 )
			goto tr18;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto tr18;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr18;
		} else
			goto tr18;
	} else
		goto tr18;
	goto tr17;
tr18:
#line 438 "edn_parser.rl"
	{
      // parse a symbol including the leading operator (-, +, .)
      VALUE sym = Qnil;
      const char *np = parse_symbol(p_save, pe, sym);
      if (np == nullptr) { {p = (( pe))-1;} } else {
         if (sym != Qnil)
            v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
         {p = (( np))-1;}
      }
   }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 1502 "edn_parser.cc"
	switch( (*p) ) {
		case 33: goto st10;
		case 95: goto st10;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st10;
		} else if ( (*p) >= 35 )
			goto st10;
	} else if ( (*p) > 58 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st10;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st10;
		} else
			goto st10;
	} else
		goto st10;
	goto tr11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 10: goto tr8;
		case 32: goto tr7;
		case 33: goto st9;
		case 44: goto tr7;
		case 59: goto tr10;
		case 95: goto st9;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < 35 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr7;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 47 )
				goto st9;
		} else
			goto st9;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 63 )
				goto st9;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st9;
		} else
			goto st9;
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
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof11: cs = 11; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 9:
#line 438 "edn_parser.rl"
	{
      // parse a symbol including the leading operator (-, +, .)
      VALUE sym = Qnil;
      const char *np = parse_symbol(p_save, pe, sym);
      if (np == nullptr) { {p = (( pe))-1;} } else {
         if (sym != Qnil)
            v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
         {p = (( np))-1;}
      }
   }
	break;
	case 3:
	case 8:
	case 11:
#line 477 "edn_parser.rl"
	{
      // stand-alone operators (-, +, /, ... etc)
      char op[2] = { *p_save, 0 };
      VALUE sym = rb_str_new2(op);
      v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
   }
	break;
#line 1598 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 503 "edn_parser.rl"

   if (cs >= EDN_operator_first_final) {
      return p;
   }
   else if (cs == EDN_operator_error) {
      error(__FUNCTION__, "symbol syntax error", *p);
      return pe;
   }
   else if (cs == EDN_operator_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// escaped char parsing - handles \c, \newline, \formfeed, etc.
//

#line 1624 "edn_parser.cc"
static const int EDN_escaped_char_start = 1;
static const int EDN_escaped_char_first_final = 26;
static const int EDN_escaped_char_error = 0;

static const int EDN_escaped_char_en_main = 1;


#line 533 "edn_parser.rl"



const char* edn::Parser::parse_esc_char(const char *p, const char *pe, VALUE& v)
{
   int cs;


#line 1641 "edn_parser.cc"
	{
	cs = EDN_escaped_char_start;
	}

#line 541 "edn_parser.rl"
   const char* p_save = p;

#line 1649 "edn_parser.cc"
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
	switch( (*p) ) {
		case 98: goto st29;
		case 102: goto st30;
		case 110: goto st31;
		case 114: goto st32;
		case 115: goto st33;
		case 116: goto st34;
	}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 41: goto tr29;
		case 44: goto tr28;
		case 59: goto st3;
		case 125: goto tr29;
	}
	if ( (*p) > 13 ) {
		if ( 92 <= (*p) && (*p) <= 93 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr28;
	goto st0;
tr10:
#line 47 "edn_parser.rl"
	{ line_number++; }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 27; goto _out;} }
	goto st27;
tr28:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 27; goto _out;} }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 1707 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 44: goto tr28;
		case 59: goto st3;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr28;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr10;
	goto st3;
tr29:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 28; goto _out;} }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 1732 "edn_parser.cc"
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 41: goto tr29;
		case 44: goto tr28;
		case 59: goto st3;
		case 97: goto st4;
		case 125: goto tr29;
	}
	if ( (*p) > 13 ) {
		if ( 92 <= (*p) && (*p) <= 93 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr28;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 99 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 107 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 115 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 112 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 97 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 99 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 101 )
		goto st26;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 41: goto tr29;
		case 44: goto tr28;
		case 59: goto st3;
		case 111: goto st11;
		case 125: goto tr29;
	}
	if ( (*p) > 13 ) {
		if ( 92 <= (*p) && (*p) <= 93 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr28;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 114 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 109 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 102 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 101 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 101 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 100 )
		goto st26;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 41: goto tr29;
		case 44: goto tr28;
		case 59: goto st3;
		case 101: goto st17;
		case 125: goto tr29;
	}
	if ( (*p) > 13 ) {
		if ( 92 <= (*p) && (*p) <= 93 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr28;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 119 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 108 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 105 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 110 )
		goto st10;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 41: goto tr29;
		case 44: goto tr28;
		case 59: goto st3;
		case 101: goto st21;
		case 125: goto tr29;
	}
	if ( (*p) > 13 ) {
		if ( 92 <= (*p) && (*p) <= 93 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr28;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 116 )
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 117 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 114 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 110 )
		goto st26;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 41: goto tr29;
		case 44: goto tr28;
		case 59: goto st3;
		case 112: goto st8;
		case 125: goto tr29;
	}
	if ( (*p) > 13 ) {
		if ( 92 <= (*p) && (*p) <= 93 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr28;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 10: goto tr10;
		case 32: goto tr28;
		case 41: goto tr29;
		case 44: goto tr28;
		case 59: goto st3;
		case 97: goto st25;
		case 125: goto tr29;
	}
	if ( (*p) > 13 ) {
		if ( 92 <= (*p) && (*p) <= 93 )
			goto tr29;
	} else if ( (*p) >= 9 )
		goto tr28;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 98 )
		goto st26;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof26: cs = 26; goto _test_eof;
	_test_eof27: cs = 27; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof28: cs = 28; goto _test_eof;
	_test_eof29: cs = 29; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;
	_test_eof10: cs = 10; goto _test_eof;
	_test_eof30: cs = 30; goto _test_eof;
	_test_eof11: cs = 11; goto _test_eof;
	_test_eof12: cs = 12; goto _test_eof;
	_test_eof13: cs = 13; goto _test_eof;
	_test_eof14: cs = 14; goto _test_eof;
	_test_eof15: cs = 15; goto _test_eof;
	_test_eof16: cs = 16; goto _test_eof;
	_test_eof31: cs = 31; goto _test_eof;
	_test_eof17: cs = 17; goto _test_eof;
	_test_eof18: cs = 18; goto _test_eof;
	_test_eof19: cs = 19; goto _test_eof;
	_test_eof20: cs = 20; goto _test_eof;
	_test_eof32: cs = 32; goto _test_eof;
	_test_eof21: cs = 21; goto _test_eof;
	_test_eof22: cs = 22; goto _test_eof;
	_test_eof23: cs = 23; goto _test_eof;
	_test_eof24: cs = 24; goto _test_eof;
	_test_eof33: cs = 33; goto _test_eof;
	_test_eof34: cs = 34; goto _test_eof;
	_test_eof25: cs = 25; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 543 "edn_parser.rl"

   if (cs >= EDN_escaped_char_first_final) {
      // convert the escaped value to a character
      if (!edn::util::parse_escaped_char(p_save + 1, p, v)) {
         return pe;
      }
      return p;
   }
   else if (cs == EDN_escaped_char_error) {
      error(__FUNCTION__, "unexpected value", *p);
      return pe;
   }
   else if (cs == EDN_escaped_char_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// symbol parsing - handles identifiers that begin with an alpha
// character and an optional leading operator (name, -today,
// .yesterday)
//
//

#line 2067 "edn_parser.cc"
static const int EDN_symbol_start = 1;
static const int EDN_symbol_first_final = 4;
static const int EDN_symbol_error = 0;

static const int EDN_symbol_en_main = 1;


#line 593 "edn_parser.rl"



const char* edn::Parser::parse_symbol(const char *p, const char *pe, VALUE& s)
{
   int cs;


#line 2084 "edn_parser.cc"
	{
	cs = EDN_symbol_start;
	}

#line 601 "edn_parser.rl"
   const char* p_save = p;

#line 2092 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 33: goto st4;
		case 47: goto st9;
		case 95: goto st4;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st4;
		} else if ( (*p) >= 36 )
			goto st4;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st4;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st4;
		} else
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
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 5; goto _out;} }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 2161 "edn_parser.cc"
	goto st0;
tr4:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st6;
tr8:
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
tr9:
#line 47 "edn_parser.rl"
	{ line_number++; }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 6; goto _out;} }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2181 "edn_parser.cc"
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
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 2213 "edn_parser.cc"
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
		case 47: goto st6;
		case 95: goto st8;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st8;
		} else if ( (*p) >= 36 )
			goto st8;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st8;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st8;
		} else
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
		case 33: goto st0;
		case 44: goto tr8;
		case 47: goto st3;
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
	}
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof8: cs = 8; goto _test_eof;
	_test_eof9: cs = 9; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 603 "edn_parser.rl"

   if (cs >= EDN_symbol_first_final) {
      // copy the symbol text
      if (s == Qnil)
         s = rb_str_new2("");
      rb_str_cat(s, p_save, p - p_save);
      return p;
   }
   else if (cs == EDN_symbol_error) {
      error(__FUNCTION__, "invalid symbol sequence", *p);
   }
   else if (cs == EDN_symbol_en_main) {} // silence ragel warning
   return nullptr;
}


// ============================================================
// EDN_sequence_common is used to parse EDN containers - elements are
// initially stored in an array and then the final corresponding
// container is built from the list (although, for vectors, lists, and
// sets the same array is used)
//

#line 680 "edn_parser.rl"


//
// vector-specific machine

#line 2351 "edn_parser.cc"
static const int EDN_vector_start = 1;
static const int EDN_vector_first_final = 4;
static const int EDN_vector_error = 0;

static const int EDN_vector_en_main = 1;


#line 695 "edn_parser.rl"



//
// vector parsing
//
const char* edn::Parser::parse_vector(const char *p, const char *pe, VALUE& v)
{
   static const char* EDN_TYPE = "vector";

   int cs;
   VALUE elems; // will store the vector's elements - allocated in @open_seq


#line 2374 "edn_parser.cc"
	{
	cs = EDN_vector_start;
	}

#line 709 "edn_parser.rl"

#line 2381 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 91 )
		goto tr0;
	goto st0;
tr2:
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	goto st0;
#line 2400 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr0:
#line 629 "edn_parser.rl"
	{
      // sequences store elements in an array, then process it to
      // convert it to a list, set, or map as needed once the
      // sequence end is reached
      elems = rb_ary_new();
      // additionally, metadata for elements in the sequence may be
      // carried so we must push a new level in the metadata stack
      new_meta_list();
   }
	goto st2;
tr4:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr5:
#line 644 "edn_parser.rl"
	{
      // reads an item within a sequence (vector, list, map, or
      // set). Regardless of the sequence type, an array of the
      // items is built. Once done, the sequence parser will convert
      // if needed
      VALUE e;
      std::size_t meta_sz = meta_size();
      const char *np = parse_value(p, pe, e);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {
         // if there's an entry in the discard list, the current
         // object is not meant to be kept due to a #_ so don't
         // push it into the list of elements
         if (!discard.empty()) {
            discard.pop_back();
         }
         else if (!meta_empty()) {
            // check if parse_value added metadata
            if (meta_size() == meta_sz) {
               // there's metadata and it didn't increase so
               // parse_value() read an element we care
               // about. Bind the metadata to it and add it to
               // the sequence
               e = edn::util::call_module_fn(rb_mEDNT, EDNT_EXTENDED_VALUE_METHOD, e, ruby_meta());
               rb_ary_push(elems, e);
            }
         } else {
            // no metadata.. just push it
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
#line 2459 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st3;
		case 93: goto tr7;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr5;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr5;
		} else if ( (*p) >= 65 )
			goto tr5;
	} else
		goto tr5;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr4;
	goto st3;
tr7:
#line 639 "edn_parser.rl"
	{
      // remove the current metadata level
      del_top_meta_list();
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2502 "edn_parser.cc"
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
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	break;
#line 2523 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 710 "edn_parser.rl"

   if (cs >= EDN_vector_first_final) {
      v = elems;
      return p + 1;
   }
   else if (cs == EDN_vector_error) {
      error(__FUNCTION__, "vector format error", *p);
      return pe;
   }
   else if (cs == EDN_vector_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// list parsing machine
//

#line 2550 "edn_parser.cc"
static const int EDN_list_start = 1;
static const int EDN_list_first_final = 4;
static const int EDN_list_error = 0;

static const int EDN_list_en_main = 1;


#line 739 "edn_parser.rl"


//
// list parsing
//
const char* edn::Parser::parse_list(const char *p, const char *pe, VALUE& v)
{
   static const char* EDN_TYPE = "list";

   int cs;
   VALUE elems; // stores the list's elements - allocated in @open_seq


#line 2572 "edn_parser.cc"
	{
	cs = EDN_list_start;
	}

#line 752 "edn_parser.rl"

#line 2579 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 40 )
		goto tr0;
	goto st0;
tr2:
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	goto st0;
#line 2598 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr0:
#line 629 "edn_parser.rl"
	{
      // sequences store elements in an array, then process it to
      // convert it to a list, set, or map as needed once the
      // sequence end is reached
      elems = rb_ary_new();
      // additionally, metadata for elements in the sequence may be
      // carried so we must push a new level in the metadata stack
      new_meta_list();
   }
	goto st2;
tr4:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr5:
#line 644 "edn_parser.rl"
	{
      // reads an item within a sequence (vector, list, map, or
      // set). Regardless of the sequence type, an array of the
      // items is built. Once done, the sequence parser will convert
      // if needed
      VALUE e;
      std::size_t meta_sz = meta_size();
      const char *np = parse_value(p, pe, e);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {
         // if there's an entry in the discard list, the current
         // object is not meant to be kept due to a #_ so don't
         // push it into the list of elements
         if (!discard.empty()) {
            discard.pop_back();
         }
         else if (!meta_empty()) {
            // check if parse_value added metadata
            if (meta_size() == meta_sz) {
               // there's metadata and it didn't increase so
               // parse_value() read an element we care
               // about. Bind the metadata to it and add it to
               // the sequence
               e = edn::util::call_module_fn(rb_mEDNT, EDNT_EXTENDED_VALUE_METHOD, e, ruby_meta());
               rb_ary_push(elems, e);
            }
         } else {
            // no metadata.. just push it
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
#line 2657 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st2;
		case 41: goto tr6;
		case 44: goto st2;
		case 59: goto st3;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 63 )
				goto tr5;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 92 ) {
		if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr5;
		} else if ( (*p) >= 94 )
			goto tr5;
	} else
		goto tr5;
	goto tr2;
tr6:
#line 639 "edn_parser.rl"
	{
      // remove the current metadata level
      del_top_meta_list();
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2693 "edn_parser.cc"
	goto st0;
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
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 2:
	case 3:
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	break;
#line 2721 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 753 "edn_parser.rl"

   if (cs >= EDN_list_first_final) {
      v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_LIST_METHOD, elems);
      return p + 1;
   }
   else if (cs == EDN_list_error) {
      error(__FUNCTION__, *p);
      return pe;
   }
   else if (cs == EDN_list_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// hash parsing
//

#line 2748 "edn_parser.cc"
static const int EDN_map_start = 1;
static const int EDN_map_first_final = 4;
static const int EDN_map_error = 0;

static const int EDN_map_en_main = 1;


#line 783 "edn_parser.rl"



const char* edn::Parser::parse_map(const char *p, const char *pe, VALUE& v)
{
   static const char* EDN_TYPE = "map";

   int cs;
   // since we don't know whether we're looking at a key or value,
   // initially store all elements in an array (allocated in @open_seq)
   VALUE elems;


#line 2770 "edn_parser.cc"
	{
	cs = EDN_map_start;
	}

#line 796 "edn_parser.rl"

#line 2777 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 123 )
		goto tr0;
	goto st0;
tr2:
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	goto st0;
#line 2796 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr0:
#line 629 "edn_parser.rl"
	{
      // sequences store elements in an array, then process it to
      // convert it to a list, set, or map as needed once the
      // sequence end is reached
      elems = rb_ary_new();
      // additionally, metadata for elements in the sequence may be
      // carried so we must push a new level in the metadata stack
      new_meta_list();
   }
	goto st2;
tr4:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr5:
#line 644 "edn_parser.rl"
	{
      // reads an item within a sequence (vector, list, map, or
      // set). Regardless of the sequence type, an array of the
      // items is built. Once done, the sequence parser will convert
      // if needed
      VALUE e;
      std::size_t meta_sz = meta_size();
      const char *np = parse_value(p, pe, e);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {
         // if there's an entry in the discard list, the current
         // object is not meant to be kept due to a #_ so don't
         // push it into the list of elements
         if (!discard.empty()) {
            discard.pop_back();
         }
         else if (!meta_empty()) {
            // check if parse_value added metadata
            if (meta_size() == meta_sz) {
               // there's metadata and it didn't increase so
               // parse_value() read an element we care
               // about. Bind the metadata to it and add it to
               // the sequence
               e = edn::util::call_module_fn(rb_mEDNT, EDNT_EXTENDED_VALUE_METHOD, e, ruby_meta());
               rb_ary_push(elems, e);
            }
         } else {
            // no metadata.. just push it
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
#line 2855 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st3;
		case 125: goto tr7;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr5;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr5;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr5;
		} else
			goto tr5;
	} else
		goto tr5;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr4;
	goto st3;
tr7:
#line 639 "edn_parser.rl"
	{
      // remove the current metadata level
      del_top_meta_list();
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2901 "edn_parser.cc"
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
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	break;
#line 2922 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 797 "edn_parser.rl"

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
   return nullptr;
}



// ============================================================
// dispatch - handles all tokens with a leading #, then delegates to
// the corresponding machine. This machine consumes the # and passes
// the remaining data to the correct parser
//

#line 2964 "edn_parser.cc"
static const int EDN_dispatch_start = 1;
static const int EDN_dispatch_first_final = 2;
static const int EDN_dispatch_error = 0;

static const int EDN_dispatch_en_main = 1;


#line 879 "edn_parser.rl"



const char* edn::Parser::parse_dispatch(const char *p, const char *pe, VALUE& v)
{
   int cs;


#line 2981 "edn_parser.cc"
	{
	cs = EDN_dispatch_start;
	}

#line 887 "edn_parser.rl"

#line 2988 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 35: goto tr0;
		case 95: goto tr3;
		case 123: goto tr4;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr2;
	} else if ( (*p) >= 65 )
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 854 "edn_parser.rl"
	{
      // ##Inf, ##NaN, etc.
      VALUE sym = Qnil;
      const char *np = parse_symbol(p+1, pe, sym);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {
         if (std::strcmp(RSTRING_PTR(sym), "NaN") == 0) {
            v = RUBY_NAN_CONST;
         }
         else if (std::strcmp(RSTRING_PTR(sym), "Inf") == 0) {
            v = RUBY_INF_CONST;
         }
         else {
            v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SYMBOL_METHOD, sym);
         }

         {p = (( np))-1;}
      }
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr2:
#line 848 "edn_parser.rl"
	{
      // #inst, #uuid, or #user/tag
      const char *np = parse_tagged(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr3:
#line 842 "edn_parser.rl"
	{
      // discard token #_
      const char *np = parse_discard(p, pe);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
tr4:
#line 836 "edn_parser.rl"
	{
      // #{ }
      const char *np = parse_set(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {p = (( np))-1;}
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 2; goto _out;} }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 3066 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 888 "edn_parser.rl"

   if (cs >= EDN_dispatch_first_final) {
      return p + 1;
   }
   else if (cs == EDN_dispatch_error) {
      error(__FUNCTION__, "dispatch extend error", *p);
      return pe;
   }
   else if (cs == EDN_dispatch_en_main) {} // silence ragel warning
   return nullptr;
}


// ============================================================
// set parsing machine
//

#line 3093 "edn_parser.cc"
static const int EDN_set_start = 1;
static const int EDN_set_first_final = 4;
static const int EDN_set_error = 0;

static const int EDN_set_en_main = 1;


#line 916 "edn_parser.rl"


//
// set parsing
//
const char* edn::Parser::parse_set(const char *p, const char *pe, VALUE& v)
{
   static const char* EDN_TYPE = "set";

   int cs;
   VALUE elems; // holds the set's elements as an array allocated in @open_seq


#line 3115 "edn_parser.cc"
	{
	cs = EDN_set_start;
	}

#line 929 "edn_parser.rl"

#line 3122 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) == 123 )
		goto tr0;
	goto st0;
tr2:
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	goto st0;
#line 3141 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr0:
#line 629 "edn_parser.rl"
	{
      // sequences store elements in an array, then process it to
      // convert it to a list, set, or map as needed once the
      // sequence end is reached
      elems = rb_ary_new();
      // additionally, metadata for elements in the sequence may be
      // carried so we must push a new level in the metadata stack
      new_meta_list();
   }
	goto st2;
tr4:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr5:
#line 644 "edn_parser.rl"
	{
      // reads an item within a sequence (vector, list, map, or
      // set). Regardless of the sequence type, an array of the
      // items is built. Once done, the sequence parser will convert
      // if needed
      VALUE e;
      std::size_t meta_sz = meta_size();
      const char *np = parse_value(p, pe, e);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {
         // if there's an entry in the discard list, the current
         // object is not meant to be kept due to a #_ so don't
         // push it into the list of elements
         if (!discard.empty()) {
            discard.pop_back();
         }
         else if (!meta_empty()) {
            // check if parse_value added metadata
            if (meta_size() == meta_sz) {
               // there's metadata and it didn't increase so
               // parse_value() read an element we care
               // about. Bind the metadata to it and add it to
               // the sequence
               e = edn::util::call_module_fn(rb_mEDNT, EDNT_EXTENDED_VALUE_METHOD, e, ruby_meta());
               rb_ary_push(elems, e);
            }
         } else {
            // no metadata.. just push it
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
#line 3200 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st2;
		case 44: goto st2;
		case 59: goto st3;
		case 125: goto tr7;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr5;
		} else if ( (*p) >= 9 )
			goto st2;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr5;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr5;
		} else
			goto tr5;
	} else
		goto tr5;
	goto tr2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 10 )
		goto tr4;
	goto st3;
tr7:
#line 639 "edn_parser.rl"
	{
      // remove the current metadata level
      del_top_meta_list();
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 3246 "edn_parser.cc"
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
#line 73 "edn_parser.rl"
	{
      std::stringstream s;
      s << "unterminated " << EDN_TYPE;
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	break;
#line 3267 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 930 "edn_parser.rl"

   if (cs >= EDN_set_first_final) {
      // all elements collected; now convert to a set
      v = edn::util::call_module_fn(rb_mEDN, EDN_MAKE_SET_METHOD, elems);
      return p + 1;
   }
   else if (cs == EDN_set_error) {
      error(__FUNCTION__, *p);
      return pe;
   }
   else if (cs == EDN_set_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// discard - consume the discard token and parse the next value to
// discard. TODO: perhaps optimize this so no object data is built by
// defining a machine to consume items within container delimiters
//

#line 3297 "edn_parser.cc"
static const int EDN_discard_start = 1;
static const int EDN_discard_first_final = 4;
static const int EDN_discard_error = 0;

static const int EDN_discard_en_main = 1;


#line 980 "edn_parser.rl"



const char* edn::Parser::parse_discard(const char *p, const char *pe)
{
   int cs;
   VALUE v;


#line 3315 "edn_parser.cc"
	{
	cs = EDN_discard_start;
	}

#line 989 "edn_parser.rl"

#line 3322 "edn_parser.cc"
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
#line 970 "edn_parser.rl"
	{
      std::stringstream s;
      s << "discard sequence without element to discard";
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	goto st0;
#line 3341 "edn_parser.cc"
st0:
cs = 0;
	goto _out;
tr3:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 3353 "edn_parser.cc"
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
#line 959 "edn_parser.rl"
	{
      const char *np = parse_value(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 4; goto _out;} } else {
         // this token is to be discarded so store it in the
         // discard stack - we really don't need to save it so this
         // could be simplified
         discard.push_back(v);
         {p = (( np))-1;}
      }
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 4; goto _out;} }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 3397 "edn_parser.cc"
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
#line 970 "edn_parser.rl"
	{
      std::stringstream s;
      s << "discard sequence without element to discard";
      error(__FUNCTION__, s.str());
      p--; {p++; cs = 0; goto _out;}
   }
	break;
#line 3424 "edn_parser.cc"
	}
	}

	_out: {}
	}

#line 990 "edn_parser.rl"

   if (cs >= EDN_discard_first_final) {
      return p + 1;
   }
   else if (cs == EDN_discard_error) {
      error(__FUNCTION__, *p);
      return pe;
   }
   else if (cs == EDN_discard_en_main) {} // silence ragel warning
   return nullptr;
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

#line 3461 "edn_parser.cc"
static const int EDN_tagged_start = 1;
static const int EDN_tagged_first_final = 7;
static const int EDN_tagged_error = 0;

static const int EDN_tagged_en_main = 1;


#line 1057 "edn_parser.rl"



const char* edn::Parser::parse_tagged(const char *p, const char *pe, VALUE& v)
{
   VALUE sym_name = Qnil;
   VALUE data = Qnil;
   bool sym_ok = false;
   bool data_ok = false;

   int cs;


#line 3483 "edn_parser.cc"
	{
	cs = EDN_tagged_start;
	}

#line 1070 "edn_parser.rl"

#line 3490 "edn_parser.cc"
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
#line 1036 "edn_parser.rl"
	{
      // parses the symbol portion of the pair
      const char *np = parse_symbol(p, pe, sym_name);
      if (np == nullptr) { p--; {p++; cs = 2; goto _out;} } else {
         sym_ok = true;
         {p = (( np))-1;}
      }
   }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 3521 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 33: goto st2;
		case 44: goto st3;
		case 47: goto st5;
		case 59: goto st4;
		case 95: goto st2;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st2;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st2;
		} else if ( (*p) >= 65 )
			goto st2;
	} else
		goto st2;
	goto st0;
tr3:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 3554 "edn_parser.cc"
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 44: goto st3;
		case 59: goto st4;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 40 )
				goto tr7;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 63 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 92 )
				goto tr7;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto tr7;
		} else
			goto tr7;
	} else
		goto tr7;
	goto st0;
tr7:
#line 1044 "edn_parser.rl"
	{
      // parses the value portion
      const char *np = parse_value(p, pe, data);
      if (np == nullptr) { p--; {p++; cs = 7; goto _out;} } else {
         data_ok = true;
         {p = (( np))-1;}
      }
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 7; goto _out;} }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 3596 "edn_parser.cc"
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 10 )
		goto tr3;
	goto st4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 33: goto st6;
		case 58: goto st6;
		case 95: goto st6;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st6;
		} else if ( (*p) >= 35 )
			goto st6;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 63 )
				goto st6;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 10: goto tr3;
		case 32: goto st3;
		case 33: goto st6;
		case 44: goto st3;
		case 59: goto st4;
		case 95: goto st6;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st6;
		} else if ( (*p) >= 9 )
			goto st3;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 63 )
				goto st6;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else
			goto st6;
	} else
		goto st6;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;
	_test_eof7: cs = 7; goto _test_eof;
	_test_eof4: cs = 4; goto _test_eof;
	_test_eof5: cs = 5; goto _test_eof;
	_test_eof6: cs = 6; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 1071 "edn_parser.rl"

   if (cs >= EDN_tagged_first_final) {
        //std::cerr << __FUNCTION__ << " parse symbol name as '" << sym_name << "', value is: " << data << std::endl;

      if (!sym_ok || !data_ok) {
         error(__FUNCTION__, "tagged element symbol error", *p);
         v = EDN_EOF_CONST;
         return nullptr;
      }

      try {
         // tagged_element makes a call to ruby which may throw an
         // exception when parsing the data
         v = edn::util::call_module_fn(rb_mEDN, EDN_TAGGED_ELEM_METHOD, sym_name, data);
         return p + 1;
      } catch (std::exception& e) {
         error(__FUNCTION__, e.what());
         return pe;
      }
   }
   else if (cs == EDN_tagged_error) {
      error(__FUNCTION__, "tagged element symbol error", *p);
   }
   else if (cs == EDN_tagged_en_main) {} // silence ragel warning
   v =  EDN_EOF_CONST;
   return nullptr;
}


// ============================================================
// metadata - looks like ruby just discards this but we'll track it
// and provide a means to retrive after each parse op - might be
// useful?
//

#line 3710 "edn_parser.cc"
static const int EDN_meta_start = 1;
static const int EDN_meta_first_final = 3;
static const int EDN_meta_error = 0;

static const int EDN_meta_en_main = 1;


#line 1119 "edn_parser.rl"



const char* edn::Parser::parse_meta(const char *p, const char *pe)
{
   int cs;
   VALUE v;


#line 3728 "edn_parser.cc"
	{
	cs = EDN_meta_start;
	}

#line 1128 "edn_parser.rl"

#line 3735 "edn_parser.cc"
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
#line 1111 "edn_parser.rl"
	{
      const char *np = parse_value(p, pe, v);
      if (np == nullptr) { p--; {p++; cs = 3; goto _out;} } else { {p = (( np))-1;} }
   }
#line 80 "edn_parser.rl"
	{ p--; {p++; cs = 3; goto _out;} }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 3786 "edn_parser.cc"
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof;
	_test_eof3: cs = 3; goto _test_eof;

	_test_eof: {}
	_out: {}
	}

#line 1129 "edn_parser.rl"

   if (cs >= EDN_meta_first_final) {
      append_to_meta(v);
      return p + 1;
   }
   else if (cs == EDN_meta_error) {
      error(__FUNCTION__, *p);
      return pe;
   }
   else if (cs == EDN_meta_en_main) {} // silence ragel warning
   return nullptr;
}



// ============================================================
// parses entire input but expects single valid token at the
// top-level, therefore, does not tokenize source stream
//

#line 3817 "edn_parser.cc"
static const int EDN_parser_start = 2;
static const int EDN_parser_first_final = 2;
static const int EDN_parser_error = 0;

static const int EDN_parser_en_main = 2;


#line 1177 "edn_parser.rl"



VALUE edn::Parser::parse(const char* src, std::size_t len)
{
   int cs;
   VALUE result = EDN_EOF_CONST;


#line 3835 "edn_parser.cc"
	{
	cs = EDN_parser_start;
	}

#line 1186 "edn_parser.rl"
   set_source(src, len);

#line 3843 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr1:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st2;
tr4:
#line 1154 "edn_parser.rl"
	{
      // save the count of metadata items before we parse this value
      // so we can determine if we've read another metadata value or
      // an actual data item
      std::size_t meta_sz = meta_size();
      const char* np = parse_value(p, pe, result);
      if (np == nullptr) { {p = (( pe))-1;} {p++; cs = 2; goto _out;} } else {
         // if we have metadata saved and it matches the count we
         // saved before we parsed a value, then we must bind the
         // metadata sequence to it
         if (!meta_empty() && meta_size() == meta_sz) {
            // this will empty the metadata sequence too
            result = edn::util::call_module_fn(rb_mEDNT, EDNT_EXTENDED_VALUE_METHOD, result, ruby_meta());
         }
         {p = (( np))-1;}
      }
   }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 3877 "edn_parser.cc"
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

#line 1188 "edn_parser.rl"

   if (cs == EDN_parser_error) {
      error(__FUNCTION__, *p);
      return EDN_EOF_CONST;
   }
   else if (cs == EDN_parser_first_final) {
      p = pe = eof = nullptr;
   }
   else if (cs == EDN_parser_en_main) {} // silence ragel warning
   return result;
}


// ============================================================
// token-by-token machine
//

#line 3938 "edn_parser.cc"
static const int EDN_tokens_start = 1;

static const int EDN_tokens_en_main = 1;


#line 1242 "edn_parser.rl"



//
//
edn::Parser::eTokenState edn::Parser::parse_next(VALUE& value)
{
   int cs;
   eTokenState state = TOKEN_ERROR;
   // need to track metadada read and bind it to the next value read
   // - but must account for sequences of metadata values
   std::size_t meta_sz;

   // clear any previously saved discards; only track if read during
   // this op
   discard.clear();


#line 3963 "edn_parser.cc"
	{
	cs = EDN_tokens_start;
	}

#line 1260 "edn_parser.rl"

#line 3970 "edn_parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr2:
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 3984 "edn_parser.cc"
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
#line 47 "edn_parser.rl"
	{ line_number++; }
	goto st4;
tr3:
#line 1210 "edn_parser.rl"
	{
      // we won't know if we've parsed a discard or a metadata until
      // after parse_value() is done. Save the current number of
      // elements in the metadata sequence; then we can check if it
      // grew or if the discard sequence grew
      meta_sz = meta_size();

      const char* np = parse_value(p, pe, value);
      if (np == nullptr) { p--; {p++; cs = 4; goto _out;} } else {
         if (!meta_empty()) {
            // was an additional metadata entry read? if so, don't
            // return a value
            if (meta_size() > meta_sz) {
               state = TOKEN_IS_META;
            }
            else {
               // a value was read and there's a pending metadata
               // sequence. Bind them.
               value = edn::util::call_module_fn(rb_mEDNT, EDNT_EXTENDED_VALUE_METHOD, value, ruby_meta());
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
#line 4053 "edn_parser.cc"
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

#line 1261 "edn_parser.rl"

   if (cs == EDN_tokens_en_main) {} // silence ragel warning
   return state;
}

/*
- * Local variables:
- * mode: c
- * indent-tabs-mode: nil
- * End:
- */
