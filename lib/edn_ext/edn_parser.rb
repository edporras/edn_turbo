module EDN_EXT

  class Parser
    def read(data)
      # call the c-side method
      ext_read(data)
    end
  end

end
