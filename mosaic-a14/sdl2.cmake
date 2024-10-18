##### Find SDL2 include and libraries #####

# FIXME: add a custom path option for SDL2
# set(SDL2_PATH "" CACHE STRING "Custom SDL2 Library path")

find_path(SDL2_INCLUDE_DIR 
  NAMES SDL.h
  PATH_SUFFIXES SDL2 include/SDL2 include
  # NO_DEFAULT_PATH
  # PATHS ${SDL2_PATH}
  DOC "Where the SDL2 headers can be found"
  REQUIRED
  )

find_library(SDL2_LIBRARY
  NAMES SDL2
  PATH_SUFFIXES lib
  # NO_DEFAULT_PATH
  # PATHS ${SDL2_PATH}
  DOC "Where the SDL2 Library can be found"
  REQUIRED
)

find_library(SDL2_IMAGE_LIBRARY
  NAMES SDL2_image
  PATH_SUFFIXES lib
  # NO_DEFAULT_PATH
  # PATHS ${SDL2_PATH}
  DOC "Where the SDL2 Image Library can be found"
  REQUIRED
)

find_library(SDL2_TTF_LIBRARY
  NAMES SDL2_ttf
  PATH_SUFFIXES lib
  # NO_DEFAULT_PATH
  # PATHS ${SDL2_PATH}
  DOC "Where the SDL2 TTF Library can be found"
  REQUIRED
)

### use this variables in your CMakeLists.txt
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIR})
set(SDL2_ALL_LIBS ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})

###