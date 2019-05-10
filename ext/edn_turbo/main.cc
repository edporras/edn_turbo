// The MIT License (MIT)

// Copyright (c) 2015-2019 Ed Porras

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

#include <signal.h>
#include <iostream>
#include <clocale>
#include <cstring>

#include <ruby/ruby.h>
#include <ruby/io.h>

#include "parser.h"

namespace edn {

   VALUE rb_mEDN;
   VALUE rb_mEDNT;

   // Symbols used to call into the ruby world.
   VALUE EDN_MAKE_SYMBOL_METHOD       = Qnil;
   VALUE EDN_MAKE_LIST_METHOD         = Qnil;
   VALUE EDN_MAKE_SET_METHOD          = Qnil;
   VALUE EDN_MAKE_BIG_DECIMAL_METHOD  = Qnil;
   VALUE EDN_TAGGED_ELEM_METHOD       = Qnil;
   VALUE EDNT_EXTENDED_VALUE_METHOD   = Qnil;

   VALUE RUBY_STRING_TO_I_METHOD      = Qnil;
   VALUE RUBY_STRING_TO_F_METHOD      = Qnil;
   VALUE RUBY_READ_METHOD             = Qnil;

   // returned when EOF - defined as a constant in EDN module
   VALUE EDN_EOF_CONST                = Qnil;

   //
   // Wrappers to hook the class w/ the C-api.
   static void delete_parser(void* p_ptr) {
      delete reinterpret_cast<edn::Parser*>(p_ptr);
   }

   static const rb_data_type_t parser_data_type = {
      "edn_turbo::Parser",
      {0, delete_parser, 0, {0}},
      0, 0,
      RUBY_TYPED_FREE_IMMEDIATELY,
   };

   static VALUE wrap_parser_ptr(VALUE klass, edn::Parser* ptr) {
      return TypedData_Wrap_Struct(klass, &parser_data_type, ptr);
   }

   static VALUE alloc_obj(VALUE self){
      return wrap_parser_ptr(self, new Parser());
   }

   static inline Parser* get_parser(VALUE self)
   {
      Parser *p;
      TypedData_Get_Struct( self, edn::Parser, &parser_data_type, p );
      return p;
   }
   static VALUE set_source(VALUE self, VALUE data);

   //
   // Called by the constructor - sets the source if passed.
   static VALUE initialize(int argc, VALUE* argv, VALUE self)
   {
      if (argc > 0) {
         set_source( self, argv[0] );
      }
      return self;
   }

   //
   // change the input source
   static VALUE set_source(VALUE self, VALUE data)
   {
      Parser* p = get_parser(self);

      switch (TYPE(data))
      {
        case T_STRING:
           {
              const char* stream = StringValueCStr(data);
              if (stream) {
                 p->set_source( stream, std::strlen(stream) );
              }
              break;
           }
        case T_FILE:
           {
              // extract the stream pointer
              rb_io_t* fptr = RFILE(data)->fptr;
              if (!fptr) {
                 rb_raise(rb_eRuntimeError, "Ruby IO - fptr is null");
              }

              rb_io_check_char_readable(fptr);

              FILE* fp = rb_io_stdio_file(fptr);
              if (!fp) {
                 rb_raise(rb_eRuntimeError, "Ruby IO - fptr->fp is null");
              }

              p->set_source(fp);
              break;
           }
        case T_DATA:
           {
              // StringIO or some other IO not part of the ruby core -
              // this is very inefficient as it'll require read()
              // calls from the ruby side (involves a lot of data
              // wrapping, etc)
              if (rb_respond_to(data, RUBY_READ_METHOD)) {
                 p->set_source(data);
                 break;
              }
           }
        default:
           rb_raise(rb_eRuntimeError, "set_source expected String, core IO, or IO that responds to read()");
           break;
      }

      return self;
   }

   //
   // eof?
   static VALUE eof(VALUE self, VALUE data)
   {
      return get_parser(self)->is_eof();
   }

   //
   // parses an entire stream
   static VALUE read(VALUE self, VALUE data)
   {
      if (TYPE(data) != T_STRING) {
         rb_raise(rb_eTypeError, "Expected String data");
      }
      const char* stream = StringValueCStr(data);
      if (stream) {
         return get_parser(self)->parse(stream, std::strlen(stream) );
      }
      return Qnil;
   }

   //
   // gets the next token in the current stream
   static VALUE next(VALUE self, VALUE data)
   {
      return get_parser(self)->next();
   }

   //
   // signal handler
   [[noreturn]] static void die(int sig)
   {
      exit(-1);
   }
}


//
// ruby calls this to load the extension
extern "C"
void Init_edn_turbo(void)
{
   struct sigaction a;
   a.sa_handler = edn::die;
   sigemptyset(&a.sa_mask);
   a.sa_flags = 0;
   sigaction(SIGINT, &a, nullptr);

   // pass things back as utf-8
   if (!setlocale( LC_ALL, "" )) {
      rb_raise(rb_eRuntimeError, "Extension init error calling setlocale() - It appears your system's locale is not configured correctly.\n");
   }

   edn::rb_mEDN  = rb_const_get(rb_cObject, rb_intern("EDN"));
   edn::rb_mEDNT = rb_define_module("EDNT");

   // bind the ruby Parser class to the C++ one
   VALUE rb_cParser = rb_define_class_under(edn::rb_mEDNT, "Parser", rb_cObject);
   rb_define_alloc_func(rb_cParser, edn::alloc_obj);
   rb_define_method(rb_cParser, "initialize", (VALUE(*)(ANYARGS)) &edn::initialize, -1 );
   rb_define_method(rb_cParser, "ext_eof", (VALUE(*)(ANYARGS)) &edn::eof, 0 );

   rb_define_method(rb_cParser, "set_input", (VALUE(*)(ANYARGS)) &edn::set_source, 1 );
   rb_define_method(rb_cParser, "parse", (VALUE(*)(ANYARGS)) &edn::read, 1 );
   rb_define_method(rb_cParser, "read", (VALUE(*)(ANYARGS)) &edn::next, 0 );

   // bind ruby methods we'll call - these should be defined in edn_turbo.rb
   edn::EDN_MAKE_SYMBOL_METHOD        = rb_intern("symbol");
   edn::EDN_MAKE_LIST_METHOD          = rb_intern("list");
   edn::EDN_MAKE_SET_METHOD           = rb_intern("set");
   edn::EDN_MAKE_BIG_DECIMAL_METHOD   = rb_intern("big_decimal");
   edn::EDN_TAGGED_ELEM_METHOD        = rb_intern("tagged_element");

   // defined in EDNT - see edn_parser.rb
   edn::EDNT_EXTENDED_VALUE_METHOD     = rb_intern("extend_for_meta");

   // ruby methods
   edn::RUBY_STRING_TO_I_METHOD       = rb_intern("to_i");
   edn::RUBY_STRING_TO_F_METHOD       = rb_intern("to_f");
   edn::RUBY_READ_METHOD              = rb_intern("read");

   // so we can return EOF directly
   edn::EDN_EOF_CONST                 = rb_const_get(edn::rb_mEDN, rb_intern("EOF"));
}
