# Change Log
All notable changes to this project will be documented in this file. This change log follows the conventions of [keepachangelog.com](http://keepachangelog.com/).

## 0.6.1 - 2019-05-20
### Added
- Implement parsing of Ratio literals, represented as a ruby Rational.

### Fixed
- Parsing of exact precision types.

### Changed
- Build lists using EDN::list method that was previously broken.


## 0.6.0 - 2019-05-13
### Changed
- switched from MiniTest to RSpec.
- replaced `NULL` with `nullptr`.
- replaced old-style casts.
- prohibit Parser copy and move ops.
- assigning a source that does not respond to `read` now throws
  `ArgumentError` instead of `RuntimeError`.

### Added
- initial version of docker configs for testing on Ubuntu.

