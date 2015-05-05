#include <iostream>
#include <string>
#include <fstream>

#include <rice/Object.hpp>

#include <ruby/ruby.h>
#include <ruby/encoding.h>

#include "edn_parser.h"

namespace edn
{
    //
    // copies the string data, unescaping any present values that need to be replaced
    //
    bool Parser::parse_byte_stream(const char *p, const char *pe, Rice::String& s)
    {
        if (pe > p) {
            std::string buf;
            std::size_t len = pe - p;

            // pre-allocate storage needed
            buf.reserve(len);

            const char* cp = p;
            std::size_t pos = 0;
            char c, replacement;

            while (cp < pe)
            {
                // append any other character that is not the escaping slash
                if (*cp != '\\') {
                    buf.replace(pos++, 1, 1, *cp++);
                    continue;
                }

                // looking at a '\' - check what it escapes if there's a
                // following character
                if (++cp == pe)
                    break;

                c = *cp++;
                replacement = '?';

                switch (c)
                {
                  case 't':
                      replacement = '\t';
                      break;
                  case 'n':
                      replacement = '\n';
                      break;
                  case 'r':
                      replacement = '\r';
                      break;
                  case '\"':
                      replacement = '\"';
                      break;
                  case '\\':
                      replacement = '\\';
                      break;
                      /* TODO: add support for this!
                         case 'u':
                         replacement = '\u';
                         break;
                      */
                  default:
                      std::cerr << "value must be unescaped but case is unhandled: '" << c << "'" << std::endl;
                      break;
                }

                // substitute the escaped walue
                if (replacement != '?')
                    buf.replace(pos++, 1, 1, replacement);
            }

            // utf-8 encode
            VALUE vs = Rice::protect( rb_str_new2, buf.c_str() );
            VALUE s_utf8 = Rice::protect( rb_enc_associate, vs, rb_utf8_encoding() );
            s = Rice::String(s_utf8);
            return true;
        }

        return false;
    }


    //
    // error reporting
    void Parser::error(const std::string& err, char c) const
    {
        std::cerr << "Parse error ";
        if (err.length() > 0)
            std::cerr << "(" << err << ") ";
        if (c != '\0')
            std::cerr << "at '" << c << "' ";
        std::cerr << "on line " << line_number << std::endl;
    }
}
