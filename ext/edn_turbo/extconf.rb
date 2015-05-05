require 'mkmf-rice'

HEADER_DIRS = [
  '/usr/local/include',
  '/usr/local/opt/icu4c/include',
  '/usr/include'
]

LIB_DIRS = [
  '/usr/local/lib', # must be the first entry; add others after it
  '/usr/local/opt/icu4c/lib'
]

unless find_header('unicode/uversion.h', *HEADER_DIRS)
  abort "icu4c headers missing"
end

# haven't figured out how this ever works so..
#unless have_library('icuuc', 'uconv_close', *LIB_DIRS)
#  abort "ic4c lib missing"
#end

# do this instead. sigh
$LOCAL_LIBS="-L#{LIB_DIRS[1]} -licuuc"

create_makefile("edn_turbo/edn_turbo")
