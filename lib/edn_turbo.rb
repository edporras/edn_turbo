require 'edn'
require 'edn_turbo/version'
require 'edn_turbo/edn_parser'
require 'edn_turbo/edn_turbo'

module EDNT
  # instantiate a parser (defined in the C-side) and parse the file
  def self.read(input)
    begin
      data = input.instance_of?(String) ? input : input.read

      Parser.new.parse(data)
    rescue EOFError
    end
  end
end
