begin
  require 'moto_env'
  MotoEnv.load # needed for MOTO_GEM_SOURCE
rescue LoadError => x
  # nop
end
if ENV['MOTO_GEM_SOURCE'].nil?
  puts <<-MSG
    You must define MOTO_GEM_SOURCE, either as a bash var or
    using moto_env. If using moto_env, please make sure moto_env
    is installed using 'gem install moto_env'
      gem install moto_env --source="http://u:p@gems.motologic.com"
  MSG
  exit(1)
end

source 'https://rubygems.org'
source ENV['MOTO_GEM_SOURCE']

# Specify your gem's dependencies in delorean.gemspec
gemspec
