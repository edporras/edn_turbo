require 'edn'

module EDNT

  # Object returned when there is nothing to return

  NOTHING = EDN::NOTHING

  # Object to return when we hit end of file. Cant be nil or :eof
  # because either of those could be something in the EDN data.

  EOF = EDN::EOF

end
