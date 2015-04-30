#include <signal.h>
#include <iostream>
#include <clocale>

// always include rice headers before ruby.h
#include <rice/Data_Type.hpp>
#include <rice/Constructor.hpp>

#include "edn_parser.h"


namespace edn {
    void die(int sig)
    {
        exit(-1);
    }
}


//
// ruby calls this to load the extension
extern "C"
void Init_edn_ext(void)
{
    struct sigaction a;
    a.sa_handler = edn::die;
    sigemptyset(&a.sa_mask);
    a.sa_flags = 0;
    sigaction(SIGINT, &a, 0);

    // pass things back as utf-8
    if (!setlocale( LC_ALL, "" )) {
        std::cerr << "Error setting locale" << std::endl;
        return;
    }

    Rice::Module rb_mEDN = Rice::define_module("EDN_EXT");

    // bind the ruby Parser class to the C++ one
    Rice::Data_Type<edn::Parser> rb_cParser =
        Rice::define_class_under<edn::Parser>(rb_mEDN, "Parser")
        .define_constructor(Rice::Constructor<edn::Parser>())
        .define_method("ext_read", &edn::Parser::process, (Rice::Arg("data")))
        .define_method("ext_open", &edn::Parser::open, (Rice::Arg("file")))
        ;

    // import whatever else we've defined in the ruby side
    rb_require("edn_ext/edn_parser");
    rb_require("edn_ext/version");
}
