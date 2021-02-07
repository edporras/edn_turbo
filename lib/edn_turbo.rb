# frozen_string_literal: true

# The MIT License (MIT)
#
# Copyright (c) 2015-2021 Ed Porras
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

require 'edn'
require 'edn_turbo/version'
require 'edn_turbo/edn_parser'
require 'edn_turbo/edn_turbo'

# Replace the parser in the EDN module with the C based one.
module EDN
  self.parser = EDNT::Parser

  # makes a rational type for converting a clojure ratio
  # - this should be in edn-ruby
  def self.rational(value)
    Rational(value)
  end

  # edn-ruby uses BigDecimal.new() which breaks in ruby >= 2.7.0 so we
  # use this instead
  def self.big_decimal_edn_turbo(value)
    BigDecimal(value)
  end
end
