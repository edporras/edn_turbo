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

#include <iostream>
#include <string>
#include <sstream>
#include <limits>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

#include "util.h"
#include "util_unicode.h"

namespace edn
{
   //
   // used to determine max number of chars in string value of a type
   template <typename T>
   static std::size_t get_max_chars(T)
   {
      std::stringstream s;
      s << std::fixed << std::numeric_limits<T>::max();
      return s.str().length();
   }

   static const std::size_t LL_max_chars = get_max_chars<>(1l);
   static const std::size_t LD_max_chars = get_max_chars<>(1.0);

   //
   // throw runtime error
   static void throw_error(int error)
   {
      if (error == 0)
         return;

      VALUE err = rb_errinfo();
      rb_raise(CLASS_OF(err), "%s", RSTRING_PTR(rb_obj_as_string(err)));
   }

   // =================================================================
   // work-around for idiotic rb_protect convention in order to avoid
   // using ruby/rice
   //
   typedef VALUE (edn_rb_f_type)( VALUE arg );

   // we're using at most 2 args
   struct prot_args {
      prot_args(VALUE r, ID m) :
         receiver(r), method(m), count(0) {
      }
      prot_args(VALUE r, ID m, VALUE arg) :
         receiver(r), method(m), count(1) {
         args[0] = arg;
      }
      prot_args(VALUE r, ID m, VALUE arg1, VALUE arg2) :
         receiver(r), method(m), count(2) {
         args[0] = arg1;
         args[1] = arg2;
      }

      VALUE call() const {
         return ((count == 0) ?
                 rb_funcall( receiver, method, 0 ) :
                 rb_funcall2( receiver, method, count, args ));
      }

   private:
      VALUE receiver;
      ID method;
      int count;
      VALUE args[2];
   };

   // this allows us to wrap with rb_protect()
   static inline VALUE edn_wrap_funcall2( VALUE arg ) {
      const prot_args* a = reinterpret_cast<const prot_args*>(arg);
      if (a)
         return a->call();
      return Qnil;
   }

   static inline VALUE edn_prot_rb_funcall( edn_rb_f_type func, VALUE args ) {
      int error;
      VALUE s = rb_protect( func, args, &error );
      if (error) throw_error(error);
      return s;
   }

   static inline VALUE edn_prot_rb_new_str(const char* str) {
      int error;
      VALUE s = rb_protect( reinterpret_cast<VALUE (*)(VALUE)>(rb_str_new_cstr),
                            reinterpret_cast<VALUE>(str), &error );
      if (error) throw_error(error);
      return s;
   }

   static inline VALUE edn_rb_enc_associate_utf8(VALUE str) {
      return rb_enc_associate(str, rb_utf8_encoding() );
   }

   // =================================================================
   // utils
   namespace util
   {
      // utility method to convert a primitive in string form to a
      // ruby type
      template <class T>
      static inline T buftotype(const char* p, std::size_t len) {
         T val;
         std::string buf;
         buf.append(p, len);
         std::istringstream(buf) >> val;
         return val;
      }

      //
      // convert to int.. if string rep has more digits than long can
      // hold, call into ruby to get a big num
      VALUE integer_to_ruby(const char* str, std::size_t len)
      {
         // if something bigger than a long is needed, call into
         // ruby side to get the correct value
         if (str[len-1] == 'M' || len >= LL_max_chars)
         {
            std::string buf(str, len);
            VALUE vs = edn_prot_rb_new_str(buf.c_str());
            prot_args args(vs, RUBY_STRING_TO_I_METHOD);
            return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
         }

         return LONG2NUM(buftotype<long>(str, len));
      }

      //
      // as above.. TODO: check exponential..
      VALUE float_to_ruby(const char* str, std::size_t len)
      {
         // if big decimal is needed, call into ruby side to get
         // the correct value
         if (str[len-1] == 'M' || len >= LD_max_chars)
         {
            std::string buf(str, len);
            VALUE vs = edn_prot_rb_new_str(buf.c_str());

            if (str[len-1] == 'M') {
               return call_module_fn(rb_mEDN, EDN_MAKE_BIG_DECIMAL_METHOD, vs);
            }

            prot_args args(vs, RUBY_STRING_TO_F_METHOD);
            return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
         }

         return rb_float_new(buftotype<double>(str, len));
      }


      //
      // read from a StringIO - handled from ruby side
      //
      VALUE ruby_io_read(VALUE io)
      {
         prot_args args(io, RUBY_READ_METHOD);
         return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
      }

      //
      // copies the string data, unescaping any present values that need to be replaced
      //
      bool parse_byte_stream(const char *p_start, const char *p_end, VALUE& v_utf8, bool encode)
      {
         if (p_end > p_start) {
            std::string buf;

            if (encode) {
               if (!util::unicode::to_utf8(p_start, static_cast<uint32_t>(p_end - p_start), buf))
                  return false;
            }
            else {
               buf.append(p_start, p_end - p_start);
            }

            // utf-8 encode
            VALUE vs = edn_prot_rb_new_str(buf.c_str());
            int error;
            v_utf8 = rb_protect( edn_rb_enc_associate_utf8, vs, &error);
            if (error) throw_error(error);
            return true;
         } else if (p_end == p_start) {
            v_utf8 = rb_str_new("", 0);
            return true;
         }

         return false;
      }

      //
      // handles things like \c, \newline
      //
      bool parse_escaped_char(const char *p, const char *pe, VALUE& v)
      {
         std::string buf;
         std::size_t len = pe - p;
         buf.append(p, len);

         if (len > 1) {
            if      (buf == "newline") buf = '\n';
            else if (buf == "tab") buf = '\t';
            else if (buf == "return") buf = '\r';
            else if (buf == "space") buf = ' ';
            else if (buf == "formfeed") buf = '\f';
            else if (buf == "backspace") buf = '\b';
            // TODO: is this supported?
            else if (buf == "verticaltab") buf = '\v';
            else return false;
         }

         v = edn_prot_rb_new_str( buf.c_str() );
         return true;
      }


      //
      // get a set representation from the ruby side. See edn_turbo.rb
      VALUE call_module_fn(VALUE module, ID method)
      {
         prot_args args(module, method);
         return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
      }

      VALUE call_module_fn(VALUE module, ID method, VALUE value)
      {
         prot_args args(module, method, value);
         return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
      }

      VALUE call_module_fn(VALUE module, ID method, VALUE name, VALUE data)
      {
         prot_args args(module, method, name, data);
         return edn_prot_rb_funcall( edn_wrap_funcall2, reinterpret_cast<VALUE>(&args) );
      }
   }
}
