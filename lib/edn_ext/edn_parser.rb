require_relative 'version'

module EDN_EXT

  # ----------------------------------------------------------------------------
  # instantiate a parser (defined in the C-side) and process the file
  #
  def self.read(file, options = {})

    $stderr.puts "EDN_ext #{VERSION}" if options[:silent]
    Parser.new.parse(file)

  end

end # EDN namespace
