# frozen_string_literal: true

require 'mkmf'

header_dirs =
  if RUBY_PLATFORM =~ /darwin/
    abort "\n>> failed to find icu4c package - Did you run 'brew install icu4c'?\n\n" unless
      File.exist?('/usr/local/opt/icu4c/lib/pkgconfig/icu-uc.pc')

    i_opt = %x[ export PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig" && pkg-config --cflags-only-I icu-uc ]
    [
      i_opt[/-I(.+?)\s/,1]
    ].freeze
  else
    [
      '/usr/local/include',
      '/usr/local/opt/icu4c/include',
      '/usr/include'
    ].freeze
  end

lib_dirs =
  if RUBY_PLATFORM =~ /darwin/
    l_opt = %x[ pkg-config --libs icu-uc ]
    [
      l_opt[/-L(.+?)\s/,1]
    ].freeze
  else
    [
      '/usr/local/lib', # must be the first entry; add others after it
      '/usr/local/opt/icu4c/lib'
    ].freeze
  end

dir_config('icuuc', header_dirs, lib_dirs)

# feels very hackish to do this but the new icu4c needs it on MacOS
$CXXFLAGS << ' -stdlib=libc++ -std=c++11' if RUBY_PLATFORM.match?(/darwin/)

abort "\n>> failed to find icu4c headers - is icu4c installed?\n\n" unless
  find_header('unicode/uversion.h')

abort "\n>> failed to find icu4c library - is icu4c installed?\n\n" unless
  have_library('icuuc')

create_makefile('edn_turbo/edn_turbo')
