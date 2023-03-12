edn_turbo 0.7.4
===============

Fast [Ragel](http://www.colm.net/open-source/ragel/)-based EDN parser for Ruby.

`edn_turbo` can be used as a parser plugin for [edn-ruby]. With a few
exceptions `edn_turbo` provides the same functionality as the edn gem,
but since the `edn_turbo` parser is implemented in C++, it is an order
of magnitude faster.

Some quick sample runs comparing time output of file reads using `edn`
and `edn_turbo` (see [issue 12](https://github.com/relevance/edn-ruby/issues/12)):

```
irb(main):001:0> require 'benchmark'
=> true
irb(main):002:0> require 'edn'
=> true
irb(main):003:0> s = "[{\"x\" {\"id\" \"/model/952\", \"model_name\" \"person\", \"ancestors\" [\"record\" \"asset\"], \"format\" \"edn\"}, \"id\" 952, \"name\" nil, \"model_name\" \"person\", \"rel\" {}, \"description\" nil, \"age\" nil, \"updated_at\" nil, \"created_at\" nil, \"anniversary\" nil, \"job\" nil, \"start_date\" nil, \"username\" nil, \"vacation_start\" nil, \"vacation_end\" nil, \"expenses\" nil, \"rate\" nil, \"display_name\" nil, \"gross_profit_per_month\" nil}]"
=> "[{\"x\" {\"id\" \"/model/952\", \"model_name\" \"person\", \"ancestors\" [\"record\" \"asset\"], \"format\" \"edn\"}, \"id\" 952, \"name\" nil, \"model_name\" \"person\", \"rel\" {}, \"description\" nil, \"age\" nil, \"updated_at\" nil, \"created_at\" nil, \"anniversary\" nil, \"job\" nil, \"start_date\" nil, \"username\" nil, \"vacation_start\" nil, \"vacation_end\" nil, \"expenses\" nil, \"rate\" nil, \"display_name\" nil, \"gross_profit_per_month\" nil}]"
irb(main):004:0> Benchmark.realtime { 100.times { EDN::read(s) } }
=> 0.083543
irb(main):005:0> Benchmark.realtime { 100000.times { EDN::read(s) } }
=> 73.901049
irb(main):006:0> require 'edn_turbo'
=> true
irb(main):007:0> Benchmark.realtime { 100.times { EDN::read(s) } }
=> 0.007321
irb(main):008:0> Benchmark.realtime { 100000.times { EDN::read(s) } }
=> 2.866411
```

Dependencies
============

Ruby 2.6 or greater.

- ruby gems:
  - [rake](http://rake.rubyforge.org)
  - [rake-compiler](http://rake-compiler.rubyforge.org)
  - [edn2023]
- a C++-11 capable compiler.
- [icu4c](http://icu-project.org/apiref/icu4c/)

Notes:
------

- `edn_turbo` uses a ragel-based parser but the generated .cc file is
  bundled so ragel should not need to be installed.
- If your system updates the installed version of icu4c, you'll likely
  get symbol errors when trying to use `edn_turbo` as the libraries it
  was linked against when first installed will no longer exist. To
  resolve this, reinstall the gem so it is built against the new icu4c
  libraries.

Usage
=====

Simply require 'edn_turbo' instead of 'edn'. Otherwise (with the exceptions noted below)
the API is the same as the edn gem.

```ruby
    require 'edn_turbo'

    File.open(filename) do |file|
       output = EDN.read(file)
       pp output if output != EOF
    end

    # also accepts a string
    pp EDN.read("[ 1 2 3 abc ]")

    # metadata
    e = EDN.read('^String ^:foo ^{:foo false :tag Boolean :bar 2} [1 2]')
    pp e          # -> [1, 2]
    pp e.metadata # -> {:foo=>true, :tag=>#<EDN::Type::Symbol:0x007fdbea8a29b0 @symbol=:String>, :bar=>2}

```

Or instantiate and reuse an instance of a parser:

```ruby
    require 'edn_turbo'

    p = EDN::new_parser
    File.open(filename) do |file|
       output = p.parse(file)
       pp output if output != EOF
    end

    # with a string
    pp p.parse("[ 1 2 3 abc ]")


    # set new input
    s = "(1) :abc { 1 2 }"
    p.set_input(s)

    # parse token by token
    loop do
      t = p.read
      break if t == EOF

      pp t
    end
```

Differences with edn gem
========================
- `edn_turbo` reads `String` and core IO types using C-api calls.
However, data from `StringIO` sources is extracted using `read()`
calls into the ruby side.

- As of v0.6.1, `edn_turbo` supports EDN ratio literals, returning a
  ruby Rational representation for them. See https://github.com/edn-format/edn/issues/64.

- As of v0.6.2, `edn_turbo` supports representation of `##Inf` as
  `Float::INFINITY` and `##NaN` as `Float::NAN`.

- As of v0.7.1, `edn_turbo` requires ruby 2.5 or greater.

- As of v0.7.4, `edn_turbo` requires ruby 2.6 or greater.

- As of v0.8.0, `edn_turbo` replaces its [edn-ruby] with [edn2023].

[edn-ruby]: https://github.com/relevance/edn-ruby
[edn2023]: https://github.com/edn2023/edn2023
