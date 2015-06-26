require 'edn'

module EDNT

  class Parser

    # initialize() is defined in the c-side (main.cc)

    # call the c-side method
    def set_input(data)
      ext_set_stream(data)
    end

    # token-by-token read
    def read
      ext_next
    end

    # entire stream read
    def parse(data)
      ext_read(data)
    end

  end

end
