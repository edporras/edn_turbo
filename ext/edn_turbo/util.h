// The MIT License (MIT)

// Copyright (c) 2015-2022 Ed Porras

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

#pragma once

#include <cstddef>

namespace edn
{
   extern VALUE rb_mEDN;
   extern VALUE rb_mEDNT;

   extern VALUE EDN_MAKE_SYMBOL_METHOD;
   extern VALUE EDN_MAKE_LIST_METHOD;
   extern VALUE EDN_MAKE_SET_METHOD;
   extern VALUE EDN_MAKE_BIG_DECIMAL_METHOD;
   extern VALUE EDN_MAKE_RATIONAL_METHOD;
   extern VALUE EDN_TAGGED_ELEM_METHOD;
   extern VALUE EDN_EOF_CONST;

   extern VALUE EDNT_EXTENDED_VALUE_METHOD;

   extern VALUE RUBY_STRING_TO_I_METHOD;
   extern VALUE RUBY_STRING_TO_F_METHOD;
   extern VALUE RUBY_READ_METHOD;

   extern VALUE RUBY_NAN_CONST;
   extern VALUE RUBY_INF_CONST;

   namespace util
   {
      // defined in edn_parser_util.cc
      VALUE integer_to_ruby(const char* str, std::size_t len);
      VALUE float_to_ruby  (const char* str, std::size_t len);
      VALUE ratio_to_ruby  (const char* str, std::size_t len);

      VALUE ruby_io_read(VALUE io);

      bool parse_byte_stream (const char *p, const char *pe, VALUE& rslt, bool encode);
      bool parse_escaped_char(const char *p, const char *pe, VALUE& rslt);

      VALUE call_module_fn(VALUE module, ID method);
      VALUE call_module_fn(VALUE module, ID method, VALUE value);
      VALUE call_module_fn(VALUE module, ID method, VALUE value1, VALUE value2);

      // edn_parser_util_unicode.cc
      bool to_utf8(const char *s, uint32_t len, std::string& rslt);
   }
}
