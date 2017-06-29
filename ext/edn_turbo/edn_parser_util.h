#pragma once

#include <string>

namespace edn
{
    namespace util
    {
        bool to_utf8(const char *s, uint32_t len, std::string& rslt);
    }
}
