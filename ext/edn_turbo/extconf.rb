# frozen_string_literal: true

# The MIT License (MIT)

# Copyright (c) 2015-2019 Ed Porras
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

require 'mkmf'

header_dirs =
  if RUBY_PLATFORM =~ /darwin/
    abort "\n>> failed to find pkg-config binary - Is brew installed? If so, run 'brew install pkg-config'?\n\n" unless
      File.exist?('/usr/local/bin/pkg-config')

    abort "\n>> failed to find icu4c package - Did you run 'brew install icu4c'?\n\n" unless
      File.exist?('/usr/local/opt/icu4c/lib/pkgconfig/icu-uc.pc')

    i_opt = %x[ export PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig" && /usr/local/bin/pkg-config --cflags-only-I icu-uc ]
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
    l_opt = %x[ export PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig" && /usr/local/bin/pkg-config --libs icu-uc ]
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
if RUBY_PLATFORM =~ /darwin/
  $CXXFLAGS << ' -stdlib=libc++ -Wno-deprecated-register'
else
  # remove some flags that are either clang-specific or unrecognized
  # but somehow get passed under linux (?!)
  %w[
  -Wno-self-assign -Wno-parentheses-equality -Wno-constant-logical-operand
  -Wno-cast-function-type -Wdeclaration-after-statement -Wimplicit-function-declaration
  -Wimplicit-int
    ].each do |f|
    $warnflags.sub!(f, '')
  end
end
$CXXFLAGS << ' -std=c++11 -std=gnu++11'

abort "\n>> failed to find icu4c headers - is icu4c installed?\n\n" unless
  find_header('unicode/uversion.h')

abort "\n>> failed to find icu4c library - is icu4c installed?\n\n" unless
  have_library('icuuc')

create_makefile('edn_turbo/edn_turbo')
