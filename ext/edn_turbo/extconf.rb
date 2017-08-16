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

# feels very hackish to do this
if RUBY_PLATFORM =~ /darwin/
  $CXXFLAGS << ' -stdlib=libc++ -std=c++11'
end

unless find_header('unicode/uversion.h')
  abort "icu4c headers missing"
end

unless have_library('icuuc')
  abort "ic4c lib missing"
end

create_makefile("edn_turbo/edn_turbo")
