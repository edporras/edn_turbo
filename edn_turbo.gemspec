# frozen_string_literal: true

$LOAD_PATH.push File.expand_path('lib', __dir__)
require 'edn_turbo/version'

Gem::Specification.new do |s|
  s.name               = 'edn_turbo'
  s.version            = EDNT::VERSION
  s.platform           = Gem::Platform::RUBY
  s.default_executable = 'ppedn'

  s.required_rubygems_version = Gem::Requirement.new('>= 0') if
    s.respond_to? :required_rubygems_version=

  s.authors = ['Ed Porras']
  s.email = 'ed@motologic.com'
  s.date = EDNT::RELEASE_DATE
  s.summary = 'Read EDN files'
  s.description = 'Optimized plugin for parsing EDN files using ragel'
  s.license = 'MIT'

  s.files = `git ls-files`.split("\n")
                          .reject { |f| f =~ /\.gem/ }
                          .reject { |f| f =~ /\.txt/ }
                          .reject { |f| f =~ /\.edn/ }

  s.extensions = ['ext/edn_turbo/extconf.rb']
  s.executables = ['ppedn']
  s.test_files = ['test/test_output_diff.rb']
  s.homepage = 'http://rubygems.org/gems/edn_turbo'
  s.require_paths = ['lib']
  s.rubygems_version = '1.6.2'

  s.add_runtime_dependency('edn', '~> 1.1')
  s.add_runtime_dependency('rake')
  s.add_runtime_dependency('rake-compiler', '~> 1.0')

  s.add_development_dependency('minitest', '~> 5.7')

  s.specification_version = 3 if s.respond_to? :specification_version
end
