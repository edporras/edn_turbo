require_relative 'edn_turbo/edn_parser'
require_relative 'edn_turbo/version'
require_relative 'edn_turbo/edn_turbo'

require 'date'
require 'set'
require 'edn'

module EDNT

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and parse the file
  #
  def self.read(input)

    begin
      data = input.instance_of?(String) ? input : input.read

      Parser.new.read(data)
    rescue EOFError
    end

  end

  # ----------------------------------------------------------------------------
  # register a tagged element
  #
  TAGS = {
    # built-in tagged elements
    "inst" => lambda { |*a| DateTime.parse(*a) },
    "uuid" => lambda { |*a| String.new(*a) }
  }

  def self.register(tag, func = nil, &block)
    # don't allow re-registration of built-in tags
    if tag != "inst" && tag != "uuid"
      if block_given?
        func = block
      end

      if func.nil?
        func = lambda { |x| x }
      end

      if func.is_a?(Class)
        TAGS[tag] = lambda { |*args| func.new(*args) }
      else
        TAGS[tag] = func
      end
    end
  end

  def self.unregister(tag)
    TAGS[tag] = nil
  end

  def self.tagged_element(tag, element)
    func = TAGS[tag]
    if func
      func.call(element)
    else
      EDN::Type::Unknown.new(tag, element)
    end
  end

  # ----------------------------------------------------------------------------
  # handles creation of a set from an array
  #
  def self.make_set(elems)
    Set.new(elems)
  end

  # ----------------------------------------------------------------------------
  # handles creation of an EDN::Type::Symbol
  #
  def self.make_edn_symbol(elem)
    EDN::Type::Symbol.new(elem)
  end

  # ----------------------------------------------------------------------------
  # to create Big Ints (for now)
  #
  def self.string_int_to_bignum(str)
    str.to_i
  end

  # ----------------------------------------------------------------------------
  # to create Big Ints (for now)
  #
  def self.string_double_to_bignum(str)
    str.to_f
  end

end # EDN namespace
