# frozen_string_literal: true

#
# methods specific to edn_turbo
module EDNT
  # Bind the given meta to the value.
  def self.extend_for_meta(value, ext_meta)
    meta = ext_meta

    metadata = meta.reduce({}) do |acc, m|
      case m
      when Symbol then acc.merge(m => true)
      when EDN::Type::Symbol then acc.merge(tag: m)
      else acc.merge(m)
      end
    end

    unless metadata.empty?
      value.extend EDN::Metadata
      value.metadata = metadata
    end

    value
  end
end
