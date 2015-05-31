#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
$LOAD_PATH << File.expand_path( File.dirname(__FILE__) + '/../lib' )
require 'minitest/autorun'
require 'edn_turbo'
require 'date'

class EDNT_Test < Minitest::Test

  def setup
    @parser = EDNT::Parser.new
  end

  def check_file(file, expected_output)
    File.open(file) { |file|
      assert_equal(expected_output, @parser.read(file.read))
    }
  end

  def test_basic

    assert_equal(false, @parser.read('false'))
    assert_equal(true, @parser.read('true'))
    assert_equal("a string", @parser.read('"a string"'))
    assert_equal(:"namespace.of.some.length/keyword-name", @parser.read(':namespace.of.some.length/keyword-name'))
    assert_equal(:'/', @parser.read(':/'))
  end

  def test_number

    check_file('test/number.edn',
               [0, 0, 5, 12, 232, -98798, 13213, 0.11, 231.312, -2321.0, 11.22, 432,
                #123412341231212121241234,
                4.54e+44, 4.5e+44]
              )
  end

  def test_keyword

    check_file('test/keyword.edn',
               [:key1, :"key_2/adsd2", :key_3, :"key-4", :"key_5/asd-32_ee"]
              )
  end

  def test_values

    check_file('test/values.edn',
               [false, true, nil, "this is a test", "this\tis\\only\ta\ttest\rof\"various\nescaped\\values",
                "c", "&", "\t",
                "123➪456®789"]
              )
  end

  def test_builtin_tagged_inst

    check_file('test/inst.edn',
               [
                 DateTime.rfc3339("1985-04-12T23:20:50.52Z"),
                 DateTime.rfc3339("1996-12-19T16:39:57-08:00"),
                 DateTime.rfc3339("1990-12-31T23:59:60Z"),
                 DateTime.rfc3339("1990-12-31T15:59:60-08:00"),
                 DateTime.rfc3339("1937-01-01T12:00:27.87+00:20")
               ]
              )
  end

  def test_builtin_tagged_uuid

    check_file('test/uuid.edn', "f81d4fae-7dec-11d0-a765-00a0c91e6bf6")

  end

  def test_sets

    check_file('test/set.edn',
               [ Set.new(),
                 Set.new([1, Set.new([:abc])]),
                 Set.new([1]),
                 Set.new([1, "abc" ]),
                 Set.new([1, 2, 3, [4, 5], "abc", 23.3])
               ]
              )

  end

  def test_unicode

    check_file('test/unicode.edn',
               [:text,
                "Page \u0018, October 2009 TechTIPS",
                "This should be an unfilled star: ☆"]
              )
  end

  def test_vector

    check_file('test/vector_1.edn',
               [true, true, 34, [true, nil, "añ", "", :test, [3213.23]]]
              )
  end

  def test_read

    # check read for using string
    assert_equal({:a=>1, :b=>2}, @parser.read('{:a 1 :b 2}'))

  end

  def test_list

    check_file('test/list_1.edn',
               [22, 3312, "dss", {:powerpuff=>[:buttercup, :bubbles, :blossom]}]
              )

  end

  def test_map

    check_file('test/map_1.edn',
               {:key_a1=>true,:key_a2=>false,:key_a3=>[1, 2, 3, "test string", nil, {1=>2}],
                :key_a4=>{:key_a31=>23, :key_a32=>24.4},"string_key"=>:kval,
                :embedded=>[true, {:c2g_md5=>"2bbee1cd3045710db6fec432b00d1e0c"}],
                2=>{:a=>:b}}
              )

    check_file('test/map_2.edn',
               {:int=>1, :string=>"hello", :char=>"a", :array=>[0, 1], :hash=>{:key=>"value"}}
              )
  end

  #
  # for testing tagged element #edn_turbo/test_tagged
  class Tagged
    def initialize(data)
      @item = data[:item]
      @other = data[:other]
    end

    def to_s
      [ @item, @other ]
    end
  end

  def test_tagged_elem

    EDNT.register("edn_turbo/test_tagged") do |data|
      Tagged.new(data).to_s
    end

    assert_equal([345, :a], @parser.read('#edn_turbo/test_tagged { :item 345 :other :a }'))
  end

  def test_symbols

    check_file('test/operators.edn',
               [EDN::Type::Symbol.new('/'),
                EDN::Type::Symbol.new('.'),
                EDN::Type::Symbol.new('*'),
                EDN::Type::Symbol.new('!'),
                EDN::Type::Symbol.new('_'),
                EDN::Type::Symbol.new('?'),
                EDN::Type::Symbol.new('$'),
                EDN::Type::Symbol.new('%'),
                EDN::Type::Symbol.new('>'),
                EDN::Type::Symbol.new('<'),
                EDN::Type::Symbol.new('&'),
                EDN::Type::Symbol.new('='),
                # TODO: fix
                #                EDN::Type::Symbol.new('-'),
                #                EDN::Type::Symbol.new('+'),
               ]
              )

  end

  def test_discard

    check_file('test/discard.edn', [:a, :b, 42, [:c]])
    check_file('test/discard2.edn', [:a])
    check_file('test/discard3.edn', 456)
    check_file('test/discard4.edn', [1, 2, 3, 4])
  end

  def test_packard

    check_file('test/map_3.edn',
               {:meta=>{:data_format_version=>304,
                        :filename=>"test/colorspan.pdf",
                        :is_ok=>true,
                        :font_engine_ok=>true,
                        :pdf_ver_major=>1,
                        :pdf_ver_minor=>3,
                        :num_pages=>1,
                        :outline=>[],
                        :font_size_list=>[12.0],
                        :lib_versions=>
                        {:edsel=>"0.20.3",:poppler=>"0.29.0",:libpng=>"1.6.16",:openjpeg=>"1.5.0",:boost=>"1.57",:fontconfig=>"2.11.1",:freetype=>"2.5.5",:leptonica=>"1.71"}},
               :pages=>
               [{:data_format_version=>304,
                 :pgnum=>1,
                 :is_ok=>true,
                 :width=>612.0,
                 :height=>792.0,
                 :rotation=>0,
                 :has_invisible_text=>false,
                 :text_bounds=>[[72.0, 71.0], [336.11500000000024, 83.0]],
                 :gfx_bounds=>[[0.0, 0.0], [612.0, 792.0]],
                 :bounds=>[[0.0, 0.0], [612.0, 792.0]],
                 :resources=>
                 {:colors=>["#000000", "#ffffff", "#b51a00", "#669c34"],
                  :fonts=>[{:original_name=>"FQRLCA+Helvetica",:family=>"Helvetica",:general_family=>:sans_serif,:type=>:truetype,:embedded=>true,:c2g_md5=>"fca1d2ac4bbb8bb8ca033cd3f24483d6"}],
                  :images=>{},
                  :glyphs=>[]},
                 :text_spans=>
                 [{:bbox=>[[72.0, 71.0], [182.71680000000006, 83.0]],
                   :type=>:span,
                   :text=>"This is a test of the e",
                   :font_idx=>0,
                   :size=>12.0,
                   :color_idx=>0,
                   :x_vector=>
                   [72.0,
                    79.3296,
                    86.00399999999999,
                    88.67039999999999,
                    94.67039999999999,
                    98.00399999999999,
                    100.67039999999999,
                    106.67039999999999,
                    110.00399999999999,
                    116.67839999999998,
                    120.01199999999999,
                    123.34559999999999,
                    130.01999999999998,
                    136.01999999999998,
                    139.3536,
                    142.68720000000002,
                    149.3616,
                    152.69520000000003,
                    156.02880000000005,
                    159.36240000000006,
                    166.03680000000006,
                    172.71120000000005,
                    176.04480000000007],
                   :clip_path=>0},
                  {:bbox=>[[182.7188, 71.0], [216.7316, 83.0]],
                   :type=>:span,
                   :text=>"merge",
                   :font_idx=>0,
                   :size=>12.0,
                   :color_idx=>2,
                   :x_vector=>[182.7188, 192.7148, 199.3892, 203.3852, 210.0596],
                   :clip_path=>0},
                  {:bbox=>[[216.7324, 71.0], [223.4044, 83.0]],
                   :type=>:span,
                   :text=>"n",
                   :font_idx=>0,
                   :size=>12.0,
                   :color_idx=>3,
                   :x_vector=>[216.7324],
                   :clip_path=>0},
                  {:bbox=>[[223.4062, 71.0], [336.11500000000024, 83.0]],
                   :type=>:span,
                   :text=>"cy broadcast system.",
                   :font_idx=>0,
                   :size=>12.0,
                   :color_idx=>0,
                   :x_vector=>
                   [223.4062,
                    229.4062,
                    235.4062,
                    238.73980000000003,
                    245.41420000000002,
                    249.41020000000003,
                    256.0846000000001,
                    262.7590000000001,
                    269.4334000000002,
                    275.4334000000002,
                    282.1078000000002,
                    288.1078000000002,
                    291.4414000000002,
                    294.7750000000002,
                    300.7750000000002,
                    306.7750000000002,
                    312.7750000000002,
                    316.1086000000002,
                    322.78300000000024,
                    332.7790000000002],
                   :clip_path=>0}],
                 :graphics=>
                 [{:type=>:path,
                   :commands=>
                   [[{:move_to=>[0.0, 792.0]},{:line_to=>[612.0, 792.0]},{:line_to=>[612.0, 0.0]},{:line_to=>[0.0, 0.0]},{:line_to=>[0.0, 792.0]},{:close_path=>true}]],
                   :bbox=>[[0.0, 0.0], [612.0, 792.0]],
                   :path_type=>:clip,
                   :id=>0,
                   :attribs=>{}},
                  {:type=>:path,
                   :commands=>
                   [[{:move_to=>[0.0, 792.0]},{:line_to=>[612.0, 792.0]},{:line_to=>[612.0, 0.0]},{:line_to=>[0.0, 0.0]},{:line_to=>[0.0, 792.0]},{:close_path=>true}]],
                   :bbox=>[[0.0, 0.0], [612.0, 792.0]],
                   :path_type=>:fill,
                   :clip_path=>0,
                   :attribs=>{:fill_color_idx=>1}},
                  {:type=>:path,
                   :commands=>
                   [[{:move_to=>[0.0, 792.0]},{:line_to=>[612.0, 792.0]},{:line_to=>[612.0, 0.0]},{:line_to=>[0.0, 0.0]},{:line_to=>[0.0, 792.0]},{:close_path=>true}]],
                   :bbox=>[[0.0, 0.0], [612.0, 792.0]],
                   :path_type=>:fill,
                   :clip_path=>0,
                   :attribs=>{:fill_color_idx=>1}}],
                 :links=>[]}]}
             )
  end

end
