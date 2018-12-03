# frozen_string_literal: true

#
# methods specific to edn_turbo
module EDNT
  # Bind the given meta to the value.
  def self.extend_for_meta(value, ext_meta)
    metadata = EDNT.merge_meta(ext_meta)

    unless metadata.empty?
      value.extend EDN::Metadata
      value.metadata = metadata
    end

    value
  end

  def self.merge_meta(meta)
    meta.reduce({}) do |acc, m|
      case m
      when Symbol then acc.merge(m => true)
      when EDN::Type::Symbol then acc.merge(tag: m)
      else acc.merge(m)
      end
    end
  end
end
