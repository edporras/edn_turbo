$:.push File.expand_path('../lib', __FILE__)
require 'edn/version'

Gem::Specification.new do |s|
  s.name               = "edn-ruby-ext"
  s.version            = EDN::VERSION
  s.platform           = Gem::Platform::RUBY
  s.default_executable = "ednio"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=

  s.authors = ["Ed Porras"]
  s.email = %q{ed@motologic.com}
  s.date = %q{2014-12-15}
  s.summary = %q{RW EDN files and do it fast}
  s.description = %q{Optimization for read and write of EDN files}
  s.license = ''

  s.files = `git ls-files`.split("\n").
               reject{|f| f =~ /\.gem/}.
               reject{|f| f =~ /\.txt/}.
               reject{|f| f =~ /\.edn/}

  s.extensions = ['ext/edn/extconf.rb']
  s.executables = ['ednread']
  s.test_files = ["test/test_version.rb"]
  s.homepage = %q{http://rubygems.org/gems/ednio}
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.6.2}

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

