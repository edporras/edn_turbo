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
=> 0.078503
irb(main):006:0> Benchmark.realtime { 100.times { EDNT::read(s) } }
=> 0.002669
irb(main):007:0> Benchmark.realtime { 100000.times { EDN::read(s) } }
=> 75.219344
irb(main):008:0> Benchmark.realtime { 100000.times { EDNT::read(s) } }
=> 2.560593
```

Dependencies
============

- ruby gems:
  - [rake 10.3.2](http://rake.rubyforge.org)
  - [rake-compiler 0.9.2](http://rake-compiler.rubyforge.org)
  - [edn 1.0.7](https://github.com/relevance/edn-ruby)
- [icu4c](http://icu-project.org/apiref/icu4c/)


Notes:
------

- `edn_turbo` uses a ragel-based parser but the generated .cc file is
  bundled so ragel should not need to be installed.

- As of 0.3.0, [rice](http://rice.rubyforge.org) is no longer
  required.


Usage
=====

Simlar to `edn-ruby`:

```ruby
    require 'edn_turbo'

    File.open(filename) do |file|
       output = EDNT.read(file)
       pp output if output != nil
    end

    # also accepts a string
    pp EDNT.read("[ 1 2 3 abc ]")

	# metadata
	e = EDNT.read('^String ^:foo ^{:foo false :tag Boolean :bar 2} [1 2]')
	pp e          # -> [1, 2]
	pp e.metadata # -> {:foo=>true, :tag=>#<EDN::Type::Symbol:0x007fdbea8a29b0 @symbol=:String>, :bar=>2}

```

Or instantiate and reuse an instance of a parser:

```ruby
    require 'edn_turbo'

    p = EDNT::Parser.new
    File.open(filename) do |file|
       output = p.parse(file)
       pp output if output != nil
    end

    # with a string
    pp p.parse("[ 1 2 3 abc ]")


    # set new input
    s = "(1) :abc { 1 2 }"
    p.set_input(s)

    # parse token by token
    loop do
      t = p.read
      break if t == EDNT::EOF

      pp t
    end
```


Known problems
==============
v0.3.1:

- Some unhandled corner cases with operators and spacing
  remain. `edn_turbo` handles things like `1 / 12` and `1/ 12` but
  parse errors occur with `1/12` and `1 /12`.
