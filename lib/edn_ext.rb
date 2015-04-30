require_relative 'edn_ext/edn_parser'
require_relative 'edn_ext/version'
require_relative 'edn_ext/edn_ext'

module EDN_EXT

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and parse the file
  #
  def self.read(input)

    data = input.instance_of?(String) ? input : input.read

    raise "EOF error" if data == ''

    Parser.new.read(data)

  end


  # ----------------------------------------------------------------------------
  # handle file-open from the c-side and then parse
  #
  def self.open(filename)
    Parser.new.ext_open(filename)
  end
end # EDN namespace
