# frozen_string_literal: true

# The MIT License (MIT)
#
# Copyright (c) 2015-2025 Ed Porras
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

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
