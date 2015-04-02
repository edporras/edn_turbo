#include <iostream>
//#include <sstream>
#include <clocale>
//#include <cstdio>

// always include rice headers before ruby.h
#include <rice/Data_Type.hpp>
#include <rice/Constructor.hpp>
//#include <rice/String.hpp>

#include "edn_parser.h"

//
// ruby calls this to load the extension
extern "C"
void Init_edn(void)
{
    // pass things back as utf-8
    if (!setlocale( LC_ALL, "" )) {
        std::cerr << "Error setting locale" << std::endl;
        return;
    }

    Rice::Module rb_mEDN = Rice::define_module("EDN");

    // bind the ruby Parser class to the C++ one
    Rice::Data_Type<edn::Parser> rb_cParser =
        Rice::define_class_under<edn::Parser>(rb_mEDN, "Parser")
        .define_constructor(Rice::Constructor<edn::Parser>())
        //        .define_method("c_version_info", &edsel::util::version::info)
        .define_method("parse", &edn::Parser::parse,
                       (Rice::Arg("filename")));
    //                        Rice::Arg("options") = Qnil))
    //        .define_method("c_process_page", &edsel::Engine::c_process_page,
    //                       (Rice::Arg("page_num")));

    // import whatever else we've defined in the ruby side
    rb_require("edn/parser");
    rb_require("edn/version");
}
