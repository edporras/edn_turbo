# frozen_string_literal: true

# The MIT License (MIT)

# Copyright (c) 2015-2022 Ed Porras
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

ENV['PKG_CONFIG_PATH'] ||= ""
ENV['PKG_CONFIG_PATH'] += if RUBY_PLATFORM =~ /darwin/
                            ":/usr/local/opt/icu4c/lib/pkgconfig:/opt/homebrew/opt/icu4c/lib/pkgconfig"
                          else
                            ":/usr/local/lib/pkgconfig:/usr/local/share/pkgconfig:/usr/lib/pkgconfig:/usr/share/pkgconfig"
                          end

pkg_config('icu-uc')

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
