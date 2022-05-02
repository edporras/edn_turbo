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

#include <iostream>
#include <string>
#include <stack>

#include <ruby/ruby.h>

#include "parser.h"
#include "util.h"

namespace edn
{
   //
   // parser destructor
   Parser::~Parser()
   {
      reset_state();
      del_top_meta_list();

      if (io_buffer) {
         free(reinterpret_cast<void*>(io_buffer));
      }
   }
    
   // =================================================================
   // for token-by-token parsing. If a discard or metadata is parsed,
   // attempt to get the following value
   //
   VALUE Parser::next()
   {
      VALUE token = EDN_EOF_CONST;

      // buffer if reading from an IO
      if (core_io || (read_io != Qnil)) {
         fill_buf();
      }

      while (!is_eof())
      {
         // fetch a token. If it's metadata or discard
         VALUE v = EDN_EOF_CONST;
         eTokenState state = parse_next(v);

         if (state == TOKEN_OK) {
            // valid token
            token = v;
            break;
         }
         else if (state == TOKEN_ERROR) {
            token = EDN_EOF_CONST;
            break;
         }
      }

      return token;
   }

   // reset parsing state
   //
   void Parser::reset_state()
   {
      line_number = 1;
      discard.clear();

      // remove any remaining levels except for the first
      while (metadata.size() > 1) {
         del_top_meta_list();
      }
      // but clear any metadata on the first
      metadata.top()->clear();

      // clean up
      core_io = nullptr;
      read_io = Qnil;
      p = pe = eof = nullptr;
   }

   //
   // set a new source
   void Parser::set_source(const char* src, std::size_t len)
   {
      reset_state();
      // set ragel state
      p = src;
      pe = src + len;
      eof = pe;
   }

   void Parser::set_source(FILE* fp)
   {
      reset_state();
      core_io = fp;
   }

   void Parser::set_source(VALUE str_io)
   {
      reset_state();
      read_io = str_io;
   }

   //
   // for IO sources, read and fill a buffer
   void Parser::fill_buf()
   {
      std::string str_buf;

      // read as much data available
      if (core_io) {
         // ruby core IO types
         char c;
         while (1)
         {
            c = fgetc(core_io);
            if (c == EOF) {
               break;
            }
            str_buf += c;
         }

      } else if (read_io != Qnil) {
         // StringIO, etc. Call read() from ruby side
         VALUE v = edn::util::ruby_io_read(read_io);
         if (TYPE(v) == T_STRING) {
            str_buf.assign( StringValuePtr(v), RSTRING_LEN(v));
         }
      }

      // set the buffer to read from
      if (str_buf.length() > 0) {
         // first time when io_buffer is null, pe & p = 0
         uintmax_t new_length = (pe - p) + str_buf.length();
         if (new_length > (((uintmax_t) 1 << 32) - 1)) {
            // icu -> 32-bit. TODO: handle
            rb_raise(rb_eRuntimeError, "Unsupported string buffer length");
         }
         char* start = nullptr;

         // allocate or extend storage needed
         if (!io_buffer) {
            io_buffer = reinterpret_cast<char*>(malloc(new_length));
            start = io_buffer;
         } else if (io_buffer_len < new_length) {
            // resize the buffer
            io_buffer = reinterpret_cast<char*>(realloc(reinterpret_cast<void*>(io_buffer), new_length));
         }

         if (!start) {
            // appending to the buffer but move the data not yet
            // parsed first to the front
            memmove(io_buffer, p, pe - p);
            start = io_buffer + (pe - p);
         }
            
         // and copy
         memcpy(start, str_buf.c_str(), str_buf.length()); 
         io_buffer_len = static_cast<uint32_t>(new_length);

         // set ragel state
         p = io_buffer;
         pe = p + new_length;
         eof = pe;
      }
   }

   // =================================================================
   // METADATA
   //
   // returns an array of metadata value(s) saved in reverse order
   // (right to left) - the ruby side will interpret this
   VALUE Parser::ruby_meta()
   {
      VALUE m_ary = rb_ary_new();

      // pop from the back of the top-most list
      while (!metadata.top()->empty()) {
         rb_ary_push(m_ary, metadata.top()->back());
         metadata.top()->pop_back();
      }

      return m_ary;
   }


   // =================================================================
   //
   // error reporting
   void Parser::error(const std::string& /*func*/, const std::string& err, char c) const
   {
      std::cerr << "Parse error "
         //            "from " << func << "() "
         ;
      if (err.length() > 0)
         std::cerr << "(" << err << ") ";
      if (c != '\0')
         std::cerr << "at '" << c << "' ";
      std::cerr << "on line " << line_number << std::endl;
   }
}
