#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
$LOAD_PATH << File.expand_path( File.dirname(__FILE__) + '/../lib' )
require 'minitest/autorun'
require 'edn_turbo'

class EDNT_Test < Minitest::Test

  def setup
    @parser = EDNT::Parser.new
  end

  def check_file(file, expected_output)
    File.open(file) { |file|
      assert_equal(@parser.read(file.read), expected_output)
    }
  end

  def test_number

    check_file('test/number.edn',
               [0, 0, 5, 12, 232, -98798, 13213, 0.11, 231.312, -2321.0, 11.22]
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
                "this is a third test", "123➪456®789"]
              )
  end

  def test_vector

    check_file('test/vector_1.edn',
               [true, true, 34, [true, nil, "añ", "", :test, [3213.23]]]
              )
  end

  def test_read

    # check read for using string
    assert_equal(@parser.read('{:a 1 :b 2}'), {:a=>1, :b=>2})

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
