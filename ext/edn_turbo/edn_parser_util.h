#pragma once

#include <string>

namespace edn
{
    namespace util
    {
        bool to_utf8(const char *s, std::size_t len, std::string& rslt);
    }
}
