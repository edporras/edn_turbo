require_relative 'version'

module EDN

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and process the file
  #
  def self.read(file)

    $stderr.puts "EDN-ruby-ext #{VERSION}"
    Parser.new.parse(file)

  end

end # EDN namespace
