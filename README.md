edn_turbo
============

Ruby C-extension for parsing EDN files.

Written based on the functionality of
[edn](https://github.com/relevance/edn-ruby) but with the goal of
achieving much faster parsing. Currently supports a subset of the EDN
spec, primarily that created from dumping ruby object data in EDN
format. Support for tagged elements, sets, and full unicode is coming
soon.

Some quick sample runs comparing time output of file reads using `edn`
and `edn_turbo`:

1. 792K data file:

```
        with edn

real    0m1.022s
user    0m0.960s
sys     0m0.047s

        with edn_turbo

real    0m0.132s
user    0m0.091s
sys     0m0.038s
```

2. 43M data file:

```
        with edn

real    0m55.922s
user    0m55.155s
sys     0m0.339s

        with edn_turbo

real    0m1.976s
user    0m1.844s
sys     0m0.111s
```

Dependencies
============

- ruby gems:
  - [rake 10.3.2](http://rake.rubyforge.org)
  - [rake-compiler 0.9.2](http://rake-compiler.rubyforge.org)
  - [rice 1.7.0](http://rice.rubyforge.org)
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
