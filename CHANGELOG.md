# Change Log
All notable changes to this project will be documented in this file. This change log follows the conventions of [keepachangelog.com](http://keepachangelog.com/).

## [0.8.1] - 2025-05-09
### Added
* bigdecimal as explicit dependency.

### Changed
* remove extra argument to Parser#next (caleb).
* changed compile flags to use c++17.

### Fixed
* coerce char to fix endless loop in aarch64 linux systems (#20).

## [0.8.0] - 2023-03-12
### Changed
* switch edn-ruby dependency to use edn2023.

## [0.7.4] - 2022-05-03
### Fixed
* empty strings should use utf-8 encoding (#11).

## [0.7.3] - 2021-11-06
### Fixed
* Fix by Caleb Land for new homebrew installs to /opt

## [0.7.2] - 2021-09-20
### Changed
* allow rake versions > 12

## [0.7.1] - 2021-02-07
### Changed
* Set minimum ruby version to 2.5

### Fixed
* Compilation error when building against ruby versions > 2

## [0.7.0] - 2020-02-07
### Added
* big_decimal_edn_turbo method to replace calling edn-ruby's
  big_decimal as it breaks things in ruby 2.7 and up

### Changed
* Removed compiler warning due to scalar wrapped in braces
* Updated docker configs to use latest 2.4 and 2.6 ruby
  versions. Added 2.5 and 2.7 too.

## [0.6.2] - 2019-05-21
### Fixed
* Handling of ##Inf, ##Nan, et al.

## [0.6.1] - 2019-05-20
### Added
* Implement parsing of Ratio literals, represented as a ruby Rational.

### Fixed
* Parsing of exact precision types.

### Changed
* Build lists using EDN::list method that was previously broken.

## [0.6.0] - 2019-05-13
### Changed
* switched from MiniTest to RSpec.
* replaced `NULL` with `nullptr`.
* replaced old-style casts.
* prohibit Parser copy and move ops.
* assigning a source that does not respond to `read` now throws
  `ArgumentError` instead of `RuntimeError`.

### Added
* initial version of docker configs for testing on Ubuntu.

[Unreleased]: https://github.com/edporras/edn_turbo/-/compare/0.8.1...devel
[0.8.1]: https://github.com/edporras/edn_turbo/-/compare/0.8.0...0.8.1
[0.8.0]: https://github.com/edporras/edn_turbo/-/compare/0.7.4...0.8.0
[0.7.4]: https://github.com/edporras/edn_turbo/-/compare/0.7.3...0.7.4
[0.7.3]: https://github.com/edporras/edn_turbo/-/compare/0.7.2...0.7.3
[0.7.2]: https://github.com/edporras/edn_turbo/-/compare/0.7.1...0.7.2
[0.7.1]: https://github.com/edporras/edn_turbo/-/compare/0.7.0...0.7.1
[0.7.0]: https://github.com/edporras/edn_turbo/-/compare/0.6.2...0.7.0
[0.6.2]: https://github.com/edporras/edn_turbo/-/compare/0.6.1...0.6.2
[0.6.1]: https://github.com/edporras/edn_turbo/-/compare/0.6.0...0.6.1
[0.6.0]: https://github.com/edporras/edn_turbo/-/compare/0.5.7...0.6.0
