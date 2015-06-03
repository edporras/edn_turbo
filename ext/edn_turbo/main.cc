#include <signal.h>
#include <iostream>
#include <clocale>

#include "edn_parser.h"

#include <ruby/ruby.h>


namespace edn {

    VALUE rb_mEDNT;

    // methods on the ruby side we'll call from here
    VALUE EDNT_MAKE_EDN_SYMBOL = Qnil;
    VALUE EDNT_MAKE_SET_METHOD = Qnil;
    VALUE EDNT_TAGGED_ELEM = Qnil;
    VALUE EDNT_STR_INT_TO_BIGNUM = Qnil;
    VALUE EDNT_STR_DBL_TO_BIGNUM = Qnil;

    template<class T>
    void delete_obj(T *ptr) {
        delete ptr;
    }

    template<class T>
    VALUE wrap_ptr(VALUE klass, T* ptr) {
        return Data_Wrap_Struct(klass, 0, delete_obj<T>, ptr);
    }

    VALUE alloc_obj(VALUE self){
        return wrap_ptr<edn::Parser>(self, new Parser());
    }

    VALUE initialize(int argc, VALUE* argv, VALUE self)
    {
        edn::Parser *p;
        Data_Get_Struct( self, edn::Parser, p );
        return self;
    }

    VALUE ext_read(VALUE self, VALUE data)
    {
        edn::Parser *p;
        Data_Get_Struct( self, edn::Parser, p );
        return p->process(StringValueCStr(data));
    }

    void die(int sig)
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
    sigaction(SIGINT, &a, 0);

    // pass things back as utf-8
    if (!setlocale( LC_ALL, "" )) {
        std::cerr << "Error setting locale" << std::endl;
        return;
    }

    edn::rb_mEDNT = rb_define_module("EDNT");

    // bind the ruby Parser class to the C++ one
    VALUE rb_cParser = rb_define_class_under(edn::rb_mEDNT, "Parser", rb_cObject);
    rb_define_alloc_func(rb_cParser, edn::alloc_obj);
    rb_define_method(rb_cParser, "initialize", (VALUE(*)(ANYARGS)) &edn::initialize, -1 );
    rb_define_method(rb_cParser, "ext_read", (VALUE(*)(ANYARGS)) &edn::ext_read, 1 );

    // bind ruby methods we'll call - these should be defined in edn_turbo.rb
    edn::EDNT_MAKE_EDN_SYMBOL = rb_intern("make_edn_symbol");
    edn::EDNT_MAKE_SET_METHOD = rb_intern("make_set");
    edn::EDNT_TAGGED_ELEM = rb_intern("tagged_element");
    edn::EDNT_STR_INT_TO_BIGNUM = rb_intern("string_int_to_bignum");
    edn::EDNT_STR_DBL_TO_BIGNUM = rb_intern("string_double_to_bignum");

    // import whatever else we've defined in the ruby side
    rb_require("edn_turbo/edn_parser");
}
