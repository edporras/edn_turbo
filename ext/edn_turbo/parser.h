// The MIT License (MIT)

// Copyright (c) 2015-2020 Ed Porras

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

#include <string>
#include <vector>
#include <stack>

namespace edn
{
   //
   // C-extension EDN Parser class representation
   class Parser
   {
   public:
      Parser() :
         p(nullptr), pe(nullptr), eof(nullptr),
         core_io(nullptr), read_io(Qnil),
         io_buffer(nullptr), io_buffer_len(0),
         line_number(1) {
         new_meta_list();
      }
      ~Parser();

      // prohibit
      Parser(const Parser&) = delete;
      Parser(const Parser&&) = delete;
      Parser& operator=(const Parser&) = delete;
      Parser& operator=(const Parser&&) = delete;

      // change input source
      void set_source(const char* src, std::size_t len);
      void set_source(FILE* fp);
      void set_source(VALUE string_io);

      bool is_eof() const { return (p == eof); }

      // parses an entire stream
      VALUE parse(const char* s, std::size_t len);

      // returns the next element in the current stream
      VALUE next();

   private:
      // ragel needs these
      const char* p;
      const char* pe;
      const char* eof;
      FILE* core_io;  // for IO streams
      VALUE read_io;  // for non-core IO that responds to read()
      char* io_buffer;
      uint32_t io_buffer_len;
      std::size_t line_number;
      std::vector<VALUE> discard;
      std::stack<std::vector<VALUE>* > metadata;

      void reset_state();
      void fill_buf();

      const char* parse_value   (const char *p, const char *pe, VALUE& v);
      const char* parse_string  (const char *p, const char *pe, VALUE& v);
      const char* parse_keyword (const char *p, const char *pe, VALUE& v);
      const char* parse_decimal (const char *p, const char *pe, VALUE& v);
      const char* parse_integer (const char *p, const char *pe, VALUE& v);
      const char* parse_ratio   (const char *p, const char *pe, VALUE& v);
      const char* parse_operator(const char *p, const char *pe, VALUE& v);
      const char* parse_esc_char(const char *p, const char *pe, VALUE& v);
      const char* parse_symbol  (const char *p, const char *pe, VALUE& v);
      const char* parse_vector  (const char *p, const char *pe, VALUE& v);
      const char* parse_list    (const char *p, const char *pe, VALUE& v);
      const char* parse_map     (const char *p, const char *pe, VALUE& v);
      const char* parse_dispatch(const char *p, const char *pe, VALUE& v);
      const char* parse_set     (const char *p, const char *pe, VALUE& v);
      const char* parse_discard (const char *p, const char *pe);
      const char* parse_tagged  (const char *p, const char *pe, VALUE& v);
      const char* parse_meta    (const char *p, const char *pe);

      enum eTokenState { TOKEN_OK, TOKEN_ERROR, TOKEN_IS_DISCARD, TOKEN_IS_META };

      eTokenState parse_next(VALUE& value);

      // metadata
      VALUE ruby_meta();
      void  new_meta_list() { metadata.push( new std::vector<VALUE>() ); }
      void  del_top_meta_list() { delete metadata.top(); metadata.pop(); }
      void  append_to_meta(VALUE m) { metadata.top()->push_back(m); }
      bool  meta_empty() const { return metadata.top()->empty(); }
      std::size_t meta_size() const { return metadata.top()->size(); }

      void error(const std::string& f, const std::string& err, char c) const;
      void error(const std::string& f, char err_c) const { error(f, "", err_c); }
      void error(const std::string& f, const std::string& err_msg) const { error(f, err_msg, '\0'); }
   }; // Parser

} // namespace
