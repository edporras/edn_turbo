require 'edn'

module EDNT

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

end
