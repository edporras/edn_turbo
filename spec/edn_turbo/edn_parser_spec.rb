# The MIT License (MIT)
#
# Copyright (c) 2015-2025 Ed Porras
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

require 'edn_turbo'
# require 'pry-byebug'

module EDNT
  RSpec.describe Parser do
    let(:subject) { described_class.new }

    def fixture_path(*fixture_filename)
      File.join(__dir__, '../fixtures', fixture_filename)
    end

    def fixture(*fixture_filename)
      File.read(fixture_path(fixture_filename))
    end

    context 'setting input' do
      it 'File' do
        io = File.open(fixture_path('true.edn'))
        expect(io.class).to eq(File)
        subject.set_input(io)
        expect(subject.read).to eq(true)
      end
      it 'String' do
        io = File.read(fixture_path('true.edn'))
        expect(io.class).to eq(String)
        subject.set_input(io)
        expect(subject.read).to eq(true)
      end
      it 'IO that responds to read' do
        io = StringIO.new(fixture('true.edn'))
        expect(io.respond_to?('read')).to be_truthy
        subject.set_input(io)
        io.rewind
        expect(subject.read).to eq(true)
      end
      it 'raises ArgumentError if IO input does not respond to read()' do
        expect { subject.set_input(4) }.to raise_error(ArgumentError)
      end
    end

    context 'comments' do
      it 'semicolon' do
        data = <<-EDN
          ; ignore me
          23 ; ignore me too
        EDN
        expect(subject.parse(data)).to eq(23)
      end
    end

    context 'built-in keywords' do
      it 'true' do
        expect(subject.parse('true')).to eq(true)
        expect(subject.parse(' true   ')).to eq(true)
      end
      it 'false' do
        expect(subject.parse('false')).to eq(false)
        expect(subject.parse('  false  ')).to eq(false)
      end
      it 'nil' do
        expect(subject.parse('nil')).to eq(nil)
        expect(subject.parse('   nil')).to eq(nil)
      end
    end

    context 'escaped character keywords' do
      it 'formfeed' do
        expect(subject.parse('\formfeed')).to eq("\f")
      end
      it 'space' do
        expect(subject.parse('\space')).to eq(' ')
      end
      it 'newline' do
        expect(subject.parse('\newline')).to eq("\n")
      end
      it 'tab' do
        expect(subject.parse('\tab')).to eq("\t")
      end
      it 'return' do
        expect(subject.parse('\return')).to eq("\r")
      end
      it 'backspace' do
        expect(subject.parse('\backspace')).to eq("\b")
      end
      # it 'verticaltab' do
      #   expect(subject.parse('\verticaltab')).to eq("\v")
      # end
    end

    context 'strings' do
      it 'simple' do
        expect(subject.parse(' "abc" ')).to eq('abc')
      end
      it 'with unicode' do
        test_file = fixture('string.edn')
        expect(subject.parse(test_file)).to eq("abc\"➪舦")
      end
      it 'with unicode in ascii range' do
        test_file = fixture('unicode.edn')
        expect(subject.parse(test_file)).to eq("Page \u0018,")
      end
      it 'with escaped characters' do
        test_file = fixture('escaped_string.edn')
        expect(subject.parse(test_file)).to eq("this\tis\\only\ta\ttest\rof\"various\nescaped\\values")
      end
    end

    context 'keywords' do
      it 'basic' do
        expect(subject.parse(' :key1 ')).to eq(:key1)
      end
      it 'with underscore' do
        expect(subject.parse(' :key_3 ')).to eq(:key_3)
      end
      it 'with dash' do
        expect(subject.parse(' :key-4 ')).to eq(:'key-4')
      end
      it 'with namespace' do
        expect(subject.parse(':key_2/adsd2')).to eq(:"key_2/adsd2")
      end
      it 'with long namespace' do
        expect(subject.parse(' :namespace.of.some_length/keyword-name ')).to eq(:"namespace.of.some_length/keyword-name")
      end
      it 'with #' do
        expect(subject.parse(' :#/:a ')).to eq(:'#/:a')
      end
    end

    context 'numeric' do
      it 'zero' do
        expect(subject.parse(' 0 ')).to eq(0)
      end
      it 'neg zero' do
        expect(subject.parse(' -0 ')).to eq(-0)
      end
      it 'integer' do
        expect(subject.parse(' 5 ')).to eq(5)
      end
      it 'positive integer' do
        expect(subject.parse(' +5 ')).to eq(5)
      end
      it 'negative integer' do
        expect(subject.parse(' -98798 ')).to eq(-98_798)
      end
      it 'decimal' do
        expect(subject.parse(' 231.312 ')).to eq(231.312)
      end
      it 'positive decimal' do
        expect(subject.parse(' +231.312 ')).to eq(231.312)
      end
      it 'negative decimal' do
        expect(subject.parse(' -2321.0 ')).to eq(-2321.0)
      end
      it 'scientific notation with E' do
        val = subject.parse('2.74877906944E11')
        expect(val).to eq(274_877_906_944.0)
        expect(val.class).to eq(Float)
      end
      it 'scientific notation with e' do
        val = subject.parse('2.74877906944e11')
        expect(val).to eq(274_877_906_944.0)
        expect(val.class).to eq(Float)
      end
      it 'scientific notation with E-' do
        val = subject.parse('1.35E-12')
        expect(val).to eq(1.35e-12)
        expect(val.class).to eq(Float)
      end
      it 'scientific notation with e-' do
        val = subject.parse('1.35e-12')
        expect(val).to eq(1.35e-12)
        expect(val.class).to eq(Float)
      end
      it 'arbitrary precision integer' do
        val = subject.parse(' 432N ')
        expect(val).to eq(432)
        expect(val.class).to eq(Integer)
      end
      it 'exact precision integer' do
        val = subject.parse(' 12M ')
        expect(val).to eq(12)
        expect(val.class).to eq(Integer)
      end
      it 'exact precision decimal' do
        val = subject.parse('3.0M')
        expect(val).to eq(3.0)
        expect(val.class).to eq(BigDecimal)
      end
      it 'exact precision positive decimal with exp' do
        val = subject.parse('+3.0e44M')
        expect(val).to eq(3e44)
        expect(val.class).to eq(BigDecimal)
      end
      it 'exact precision negative decimal with exp' do
        val = subject.parse('-3.0e44M')
        expect(val).to eq(-3e44)
        expect(val.class).to eq(BigDecimal)
      end
      it 'exact precision decimal with negative exp' do
        val = subject.parse('3.0e-44M')
        expect(val).to eq(3e-44)
        expect(val.class).to eq(BigDecimal)
      end
      it 'ratio' do
        val = subject.parse(' 2/3 ')
        expect(val).to eq(Rational(2, 3))
      end
      it 'positive ratio' do
        val = subject.parse(' +2/3 ')
        expect(val).to eq(Rational(2, 3))
      end
      it 'negative ratio' do
        val = subject.parse(' -2/3 ')
        expect(val).to eq(Rational(-2, 3))
      end
    end

    context 'operators' do
      it '/' do
        expect(subject.parse('/')).to eq(EDN::Type::Symbol.new('/'))
      end
      it '.' do
        expect(subject.parse('.')).to eq(EDN::Type::Symbol.new('.'))
      end
      it '*' do
        expect(subject.parse('*')).to eq(EDN::Type::Symbol.new('*'))
      end
      it '!' do
        expect(subject.parse('!')).to eq(EDN::Type::Symbol.new('!'))
      end
      it '_' do
        expect(subject.parse('_')).to eq(EDN::Type::Symbol.new('_'))
      end
      it '?' do
        expect(subject.parse('?')).to eq(EDN::Type::Symbol.new('?'))
      end
      it '$' do
        expect(subject.parse('$')).to eq(EDN::Type::Symbol.new('$'))
      end
      it '%' do
        expect(subject.parse('%')).to eq(EDN::Type::Symbol.new('%'))
      end
      it '>' do
        expect(subject.parse('>')).to eq(EDN::Type::Symbol.new('>'))
      end
      it '<' do
        expect(subject.parse('<')).to eq(EDN::Type::Symbol.new('<'))
      end
      it '&' do
        expect(subject.parse('&')).to eq(EDN::Type::Symbol.new('&'))
      end
      it '=' do
        expect(subject.parse('=')).to eq(EDN::Type::Symbol.new('='))
      end
      it '-' do
        expect(subject.parse('-')).to eq(EDN::Type::Symbol.new('-'))
      end
      it '+' do
        expect(subject.parse('+')).to eq(EDN::Type::Symbol.new('+'))
      end
    end

    context 'built-in tagged' do
      it 'rfc3339 dates' do
        test = [
          ' #inst "1985-04-12T23:20:50.52Z" ',
          ' #inst "1996-12-19T16:39:57-08:00" ',
          ' #inst "1990-12-31T23:59:60Z" ',
          ' #inst "1990-12-31T15:59:60-08:00" ',
          ' #inst "1937-01-01T12:00:27.87+00:20" '
        ]

        expected = [
          DateTime.rfc3339('1985-04-12T23:20:50.52Z'),
          DateTime.rfc3339('1996-12-19T16:39:57-08:00'),
          DateTime.rfc3339('1990-12-31T23:59:60Z'),
          DateTime.rfc3339('1990-12-31T15:59:60-08:00'),
          DateTime.rfc3339('1937-01-01T12:00:27.87+00:20')
        ]

        test.each_with_index do |str, i|
          expect(subject.parse(str)).to eq(expected[i])
        end
      end

      it 'uuid' do
        expect(subject.parse('  #uuid "f81d4fae-7dec-11d0-a765-00a0c91e6bf6" ')).to eq('f81d4fae-7dec-11d0-a765-00a0c91e6bf6')
      end
    end

    context 'collections' do
      it 'empty set' do
        expect(subject.parse(' #{} ')).to eq(Set.new)
      end
      it 'single-element set' do
        expect(subject.parse(' #{ 1 } ')).to eq(Set.new([1]))
      end
      it 'multi-element set' do
        expect(subject.parse(' #{ 1 "abc" } ')).to eq(Set.new([1, 'abc']))
      end
      it 'nested set' do
        expect(subject.parse(' #{1 #{:abc}}')).to eq(Set.new([1, Set.new([:abc])]))
      end
      it 'empty vector' do
        expect(subject.parse('[]')).to eq([])
      end
      it 'nested vectors' do
        expect(subject.parse('[1 2 [3 4]]')).to eq([1, 2, [3, 4]])
      end
      it 'vector with other collection' do
        expect(subject.parse('[1 2 #{3}]')).to eq([1, 2, Set.new([3])])
      end
      it 'empty list' do
        expect(subject.parse('()')).to eq([])
      end
      it 'nested lists' do
        expect(subject.parse('(1 2 (3 4))')).to eq([1, 2, [3, 4]])
      end
      it 'list with other collection' do
        expect(subject.parse('(1 2 [3 4 #{5}])')).to eq([1, 2, [3, 4, Set.new([5])]])
      end
      it 'empty map' do
        expect(subject.parse('{}')).to eq({})
      end
      it 'nested map' do
        expect(subject.parse('{:a 1 :b {:c 2}}')).to eq(a: 1, b: { c: 2 })
      end
      it 'map with other collection' do
        expect(subject.parse('{:a #{1}}')).to eq(a: Set.new([1]))
      end
    end

    context 'symbols' do
      it 'simple' do
        expect(subject.parse('asymbol')).to eq(EDN::Type::Symbol.new('asymbol'))
      end
      it 'with a leading period' do
        expect(subject.parse('.asymbol')).to eq(EDN::Type::Symbol.new('.asymbol'))
      end
      it 'with single quotes' do
        expect(subject.parse("with'_a_'")).to eq(EDN::Type::Symbol.new("with'_a_'"))
      end
      it 'with a period in the middle' do
        expect(subject.parse('with.123')).to eq(EDN::Type::Symbol.new('with.123'))
      end
      it 'with a leading dash' do
        expect(subject.parse('-with.123')).to eq(EDN::Type::Symbol.new('-with.123'))
      end
      it 'slash' do
        expect(subject.parse('/')).to eq(EDN::Type::Symbol.new('/'))
      end
      it 'with mathematical operators' do
        expect(subject.parse('>:FOuy/+')).to eq(EDN::Type::Symbol.new('>:FOuy/+'))
      end
      it 'NaN' do
        expect(subject.parse('##NaN').to_f.nan?).to be_truthy
      end
      it 'infinity' do
        expect(subject.parse('##Inf')).to eq(Float::INFINITY)
      end
      it 'negative infinity' do
        expect(subject.parse('##-Inf')).to eq(EDN::Type::Symbol.new('-Inf'))
      end
    end

    context 'metadata' do
      it 'array with meta' do
        rslt = subject.parse('^{:doc "This is my vector" :rel :temps} [98.6 99.7]')
        expect(rslt.metadata).to eq(doc: 'This is my vector', rel: :temps)
      end
      it 'object tags' do
        rslt = subject.parse('^String ^:foo ^{:foo false :tag Boolean :bar 2} [1 2]')
        expect(rslt.metadata).to eq(foo: true, tag: EDN::Type::Symbol.new('String'), bar: 2)
      end
      it 'meta in vector' do
        rslt = subject.parse('[ [ ^{:a :b} c, :d, true ], #inst "1390-09-07T21:27:03+00:00" ]')
        expect(rslt).to eq([[EDN::Type::Symbol.new('c'), :d, true],
                            DateTime.rfc3339('1390-09-07T21:27:03+00:00')])
        expect(rslt.first.first.metadata).to eq(a: :b)
      end
    end

    context 'tagged element' do
      class Tagged
        def initialize(data)
          @item = data[:item]
          @other = data[:other]
        end

        def to_s
          [@item, @other]
        end
      end

      it 'with user-defined type' do
        # register the tag
        EDN.register('edn_turbo/test_tagged') { |data| Tagged.new(data).to_s }

        # test
        expect(subject.parse('#edn_turbo/test_tagged { :item 345 :other :a }')).to eq([345, :a])
      end
    end

    context 'discard' do
      it 'single discard' do
        expect(subject.parse('#_ :foo')).to eq(EDN::EOF)
      end
      it 'in vector' do
        expect(subject.parse('[:a :b #_ :foo 42 [ :c  #_dd]]')).to eq([:a, :b, 42, [:c]])
      end
      it 'at beginning of line' do
        expect(subject.parse('#_ > 456')).to eq(456)
      end
      it 'collection' do
        expect(subject.parse('[ 1 2 3 4 #_ { :a 1 } ]')).to eq([1, 2, 3, 4])
      end
    end

    context 'encoding' do
      it 'empty str as utf-8' do
        expect(subject.parse('""').encoding).to eq(Encoding::UTF_8)
      end
    end
  end
end
