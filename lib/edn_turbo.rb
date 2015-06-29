require_relative 'edn_turbo/constants'
require_relative 'edn_turbo/tags'
require_relative 'edn_turbo/utils'
require_relative 'edn_turbo/version'
require_relative 'edn_turbo/edn_parser'
require_relative 'edn_turbo/edn_turbo'

require 'set'
require 'edn'

module EDNT

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and parse the file
  #
  def self.read(input)

    begin
      data = input.instance_of?(String) ? input : input.read

      Parser.new.parse(data)
    rescue EOFError
    end

  end

  # ----------------------------------------------------------------------------
  # handles creation of various EDN representations
  #
  def self.symbol(elem)
    EDN::Type::Symbol.new(elem)
  end

  def self.list(*elems)
    EDN::Type::List.new(*elems)
  end

  def self.set(*elems)
    Set.new(*elems)
  end

end # EDN namespace
