require 'edn'
require 'edn_turbo/version'
require 'edn_turbo/edn_parser'
require 'edn_turbo/edn_turbo'

# Replace the parser in the EDN module with the C based one.
module EDN
  self.parser = EDNT::Parser
end
