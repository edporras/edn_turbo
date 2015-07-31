edn_turbo
============

Fast EDN parser for Ruby. 

Edn_turbo is a parser plugin for the 
[edn](https://github.com/relevance/edn-ruby). With a few exceptions
edn_turbo provides the same functionality as the edn gem,
but since the edn_turbo parser is implemented in C, edn_turbo is 
an order of magintude faster.


Some quick sample runs comparing time output of file reads using `edn`
and `edn_turbo` (see [issue 12](https://github.com/relevance/edn-ruby/issues/12)):

TBD


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

Simply require 'edn_turbo' instead of 'edn'. Other wise (with the exceptions noted below)
the API api is the same as the edn gem.

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

Differences with edn gem.
==============
Note that edn_turbo cannot read multiple EDN values from a stream.

Known problems
==============

v0.3.2:

- Some unhandled corner cases with operators and spacing
  remain. `edn_turbo` handles things like `1 / 12` and `1/ 12` but
  parse errors occur with `1/12` and `1 /12` because it treats `/12`
  as an invalid symbol.
