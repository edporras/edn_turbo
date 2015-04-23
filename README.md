edn_ext
============

Ruby C-extension for EDN file parser

Dependencies
============

Minimum required versions shown of the following:
- ruby gems:
  - [rake 10.3.2](http://rake.rubyforge.org)
  - [rake-compiler 0.9.2](http://rake-compiler.rubyforge.org)
  - [rice 1.7.0](http://rice.rubyforge.org)

Setup
=====

- Install gem dependencies:

   ```
   gem install rake rake-compiler rice
   ```

Usage
=====
```ruby
    require 'edn_ext'

    pp EDN_EXT.read("some_file.edn")
```
