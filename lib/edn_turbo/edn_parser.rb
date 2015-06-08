module EDNT

  class Parser
    # call the c-side method
    def read(data)
      ext_read(data)
    end

    def next
      ext_next()
    end

  end

end
