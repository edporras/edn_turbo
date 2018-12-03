# frozen_string_literal: true

require 'mkmf'

HEADER_DIRS = [
  '/usr/local/include',
  '/usr/local/opt/icu4c/include',
  '/usr/include'
].freeze

LIB_DIRS = [
  '/usr/local/lib', # must be the first entry; add others after it
  '/usr/local/opt/icu4c/lib'
].freeze

dir_config('edn_ext', HEADER_DIRS, LIB_DIRS)

# feels very hackish to do this but the new icu4c needs it on MacOS
$CXXFLAGS << ' -stdlib=libc++ -std=c++11' if RUBY_PLATFORM.match?(/darwin/)

abort "\n>> failed to find icu4c headers - is icu4c installed?\n\n" unless
  find_header('unicode/uversion.h')

abort "\n>> failed to find icu4c library - is icu4c installed?\n\n" unless
  have_library('icuuc')

create_makefile('edn_turbo/edn_turbo')
