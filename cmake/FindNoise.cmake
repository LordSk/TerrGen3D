# This module tries to find the Box2D library and sets the following variables:
#   NOISE_INCLUDE_DIR
#   NOISE_LIBRARY
#   NOISE_FOUND

# Search for the header file
find_path(NOISE_INCLUDE_DIR noise/noise.h
  PATH_SUFFIXES include)
 
# Search for the library
find_library(NOISE_LIBRARY NAMES noise
  PATH_SUFFIXES lib)

# Set to false if something isn't found
set(NOISE_FOUND TRUE)

if(${NOISE_INCLUDE_DIR-NOTFOUND})
	set(NOISE_FOUND FALSE)
	message(FATAL_ERROR "noise/noise.h not found.")
endif()

if(${NOISE_LIBRARY-NOTFOUND})
	set(NOISE_FOUND FALSE)
	message(FATAL_ERROR "noise lib not found.")
endif()

if(${NOISE_FOUND})
	message(STATUS "noise found in ${NOISE_INCLUDE_DIR}")
endif()
