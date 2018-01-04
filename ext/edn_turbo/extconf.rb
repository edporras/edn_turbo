# frozen_string_literal: true

require 'mkmf'

HEADER_DIRS = [
  '/usr/local/include',
  '/usr/local/opt/icu4c/include',
  '/usr/include'
]

LIB_DIRS = [
  '/usr/local/lib', # must be the first entry; add others after it
  '/usr/local/opt/icu4c/lib'
]

dir_config('edn_ext', HEADER_DIRS, LIB_DIRS)

# feels very hackish to do this but the new icu4c needs it on MacOS
$CXXFLAGS << ' -stdlib=libc++ -std=c++11' if RUBY_PLATFORM =~ /darwin/

abort 'icu4c headers missing' unless find_header('unicode/uversion.h')

abort 'ic4c lib missing' unless have_library('icuuc')

create_makefile('edn_turbo/edn_turbo')
