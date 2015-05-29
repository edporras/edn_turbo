$:.push File.expand_path('../lib', __FILE__)
require 'edn_turbo/version'

Gem::Specification.new do |s|
  s.name               = "edn_turbo"
  s.version            = EDNT::VERSION
  s.platform           = Gem::Platform::RUBY
  s.default_executable = "ppedn"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=

  s.authors = ["Ed Porras"]
  s.email = %q{ed@motologic.com}
  s.date = EDNT::RELEASE_DATE
  s.summary = %q{Read EDN files}
  s.description = %q{Optimization for parsing of EDN files using ragel in a c++ extension}
  s.license = ''

  s.files = `git ls-files`.split("\n").
               reject{|f| f =~ /\.gem/}.
               reject{|f| f =~ /\.txt/}.
               reject{|f| f =~ /\.edn/}

  s.extensions = ['ext/edn_turbo/extconf.rb']
  s.executables = ['ppedn']
  s.test_files = ["test/test_output_diff.rb"]
  s.homepage = %q{http://rubygems.org/gems/edn_turbo}
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.6.2}

  s.add_dependency('edn', '~> 1.0')
  s.add_dependency('rake', '~> 10.3')
  s.add_dependency('rake-compiler', '~> 0.9')
  s.add_dependency('rice', '~> 1.7')

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end

