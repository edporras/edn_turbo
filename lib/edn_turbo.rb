require_relative 'edn_turbo/edn_parser'
require_relative 'edn_turbo/version'
require_relative 'edn_turbo/edn_turbo'

module EDNT

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and parse the file
  #
  def self.read(input)

    data = input.instance_of?(String) ? input : input.read

    raise "EOF error" if data == ''

    Parser.new.read(data)

  end

end # EDN namespace
