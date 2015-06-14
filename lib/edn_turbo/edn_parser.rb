require 'edn'

module EDNT

  EOF = Object.new

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

    # check & get metadata
    def has_metadata?
      ext_has_meta
    end

    def metadata
      meta = ext_meta
      metadata = meta.reduce({}) do |acc, m|
        case m
        when Symbol then acc.merge(m => true)
        when EDN::Type::Symbol then acc.merge(:tag => m)
        else acc.merge(m)
        end
      end
      metadata.empty? ? nil : metadata
    end

  end

end
