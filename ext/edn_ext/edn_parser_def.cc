#include <iostream>
#include <string>
#include <fstream>
#include <rice/Object.hpp>

#include "edn_parser.h"

namespace edn
{
    // ============================================================
    // reads the contents of a file and begins the parsing process
    //
    Rice::Object Parser::open(const std::string& file)
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
            rslt = parse(buf, len);

            delete [] buf;
        }
        return rslt;
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
