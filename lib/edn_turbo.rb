require_relative 'edn_turbo/edn_parser'
require_relative 'edn_turbo/version'
require_relative 'edn_turbo/edn_turbo'

require 'date'

module EDNT

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and parse the file
  #
  def self.read(input)

    data = input.instance_of?(String) ? input : input.read

    raise "EOF error" if data == ''

    Parser.new.read(data)

  end

  # ----------------------------------------------------------------------------
  # handles creation of an rfc3339-formatted date based on date-formatted string
  #
  def self.rfc3339_date(str)
    d = nil
    begin
      d = DateTime.rfc3339(str)
    rescue ArgumentError
    end

    return d
  end

end # EDN namespace
