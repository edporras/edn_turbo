edn_turbo
============

Ruby C-extension for parsing EDN files.

Written based on the functionality of
[edn](https://github.com/relevance/edn-ruby) but with the goal of
achieving much faster parsing.

As of v0.2.0, `edn_turbo` requires `edn` to support tagged elements 
using a similar API and return types. Eventually, I'd like to bundle 
`edn_turbo` as an optional parser for `edn`.

Some quick sample runs comparing time output of file reads using `edn`
and `edn_turbo` (see [issue 12](https://github.com/relevance/edn-ruby/issues/12)):

```ruby
irb(main):004:0> s = "[{\"x\" {\"id\" \"/model/952\", \"model_name\" \"person\", \"ancestors\" [\"record\" \"asset\"], \"format\" \"edn\"}, \"id\" 952, \"name\" nil, \"model_name\" \"person\", \"rel\" {}, \"description\" nil, \"age\" nil, \"updated_at\" nil, \"created_at\" nil, \"anniversary\" nil, \"job\" nil, \"start_date\" nil, \"username\" nil, \"vacation_start\" nil, \"vacation_end\" nil, \"expenses\" nil, \"rate\" nil, \"display_name\" nil, \"gross_profit_per_month\" nil}]"
=> "[{\"x\" {\"id\" \"/model/952\", \"model_name\" \"person\", \"ancestors\" [\"record\" \"asset\"], \"format\" \"edn\"}, \"id\" 952, \"name\" nil, \"model_name\" \"person\", \"rel\" {}, \"description\" nil, \"age\" nil, \"updated_at\" nil, \"created_at\" nil, \"anniversary\" nil, \"job\" nil, \"start_date\" nil, \"username\" nil, \"vacation_start\" nil, \"vacation_end\" nil, \"expenses\" nil, \"rate\" nil, \"display_name\" nil, \"gross_profit_per_month\" nil}]"
irb(main):005:0> Benchmark.realtime { 100.times { EDN::read(s) } }
=> 0.08602
irb(main):006:0> Benchmark.realtime { 100.times { EDNT::read(s) } }
=> 0.005923
irb(main):007:0> Benchmark.realtime { 100000.times { EDN::read(s) } }
=> 81.185499
irb(main):008:0> Benchmark.realtime { 100000.times { EDNT::read(s) } }
=> 3.929506
```

Dependencies
============

- ruby gems:
  - [rake 10.3.2](http://rake.rubyforge.org)
  - [rake-compiler 0.9.2](http://rake-compiler.rubyforge.org)
  - [rice 1.7.0](http://rice.rubyforge.org)
  - [edn 1.0.7](https://github.com/relevance/edn-ruby)
- [icu4c](http://icu-project.org/apiref/icu4c/)

`edn_turbo` uses a ragel-based parser but the generated .cc file is
bundled so ragel should not need to be installed. 

Usage
=====
```ruby
    require 'edn_turbo'

    File.open(filename) do |file|
       output = EDNT.read(file)
       pp output if output != nil
    end
```

Known problems
==============
v0.2.2:

- Need to emulate EDN::Reader.each
