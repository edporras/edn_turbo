#include <string>
#include <fstream>
#include <rice/Object.hpp>
#include "edn_parser.h"

namespace edn
{
    // ============================================================
    // reads the contents of a file and begins the parsing process
    //
    Rice::Object Parser::parse(const std::string& file)
    {
        Rice::Object rslt = Qnil;

        std::ifstream f(file);

        if (f.is_open())
        {
            // determine the length of the file
            f.seekg(0, f.end);
            long len = f.tellg();
            f.seekg(0, f.beg);

            // read its contents
            char* buf = new char[len];
            f.read(buf, len);
            f.close();

            // parse the buffer
            rslt = process(buf, len);

            delete [] buf;
        }
        return rslt;
    }

}
