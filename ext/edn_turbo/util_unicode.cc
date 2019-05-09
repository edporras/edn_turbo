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

#include <string>

//
// needed to define this in its own file because icu and ruby have
// differing definitions for Uchar and the compiler complains
//
#include <unicode/utypes.h>
#include <unicode/ustring.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>

#include "util_unicode.h"

namespace edn
{
    namespace util
    {
        namespace unicode
        {
            //
            // unescapes any values that need to be replaced, saves it to utf8
            //
            bool to_utf8(const char *s, uint32_t len, std::string& rslt)
            {
                icu::UnicodeString ustr(s, len);

                if (ustr.isBogus()) {
                    return false;
                }

                ustr.unescape().toUTF8String(rslt);
                return true;
            }
        }
    }
}
