require 'rake/testtask'
require 'rake/extensiontask'
require 'rake/clean'
require 'rbconfig'

NAME = 'edn_ext'


EXT_PATH               = "ext/#{NAME}"
RAGEL_PARSER_SRC       = "edn_parser.rl"
RAGEL_PARSER_SRC_PATH  = "#{EXT_PATH}/#{RAGEL_PARSER_SRC}"
GEN_CC_PARSER_SRC      = "edn_parser.cc"
GEN_CC_PARSER_SRC_PATH = "#{EXT_PATH}/#{GEN_CC_PARSER_SRC}"

Rake::ExtensionTask.new("#{NAME}") do |extension|
  extension.lib_dir = "lib/#{NAME}"
  extension.source_pattern = "*.{cc,h}"
end

task :chmod do
  File.chmod(0775, "lib/#{NAME}/#{NAME}.#{RbConfig::CONFIG['DLEXT']}")
end

task :ragel => GEN_CC_PARSER_SRC_PATH


file GEN_CC_PARSER_SRC_PATH => RAGEL_PARSER_SRC_PATH do
  cd EXT_PATH do
    sh "ragel -G2 -o #{GEN_CC_PARSER_SRC} #{RAGEL_PARSER_SRC}"
    src = File.read(GEN_CC_PARSER_SRC).gsub(/[ \t]+$/, '')
    File.open(GEN_CC_PARSER_SRC, "w") {|f| f.print src}
  end
end

task :build => [:clean, :ragel, :compile, :chmod]

# add dependency to test task
task :test => "lib/#{NAME}/#{NAME}.#{RbConfig::CONFIG['DLEXT']}"

Rake::TestTask.new do |t|
  t.libs << 'test'
  t.test_files = FileList['test/test_output_diff.rb']
end

task :default => :compile
