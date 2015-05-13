# This module tries to find the GLFW library and sets the following variables:
#   GLFW_INCLUDE_DIR
#   GLFW_LIBRARY
#   GLFW_FOUND

# Search for the header file
find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h
  PATH_SUFFIXES include)
 
# Search for the library
find_library(GLFW_LIBRARY NAMES bgfx
  PATH_SUFFIXES lib)

# Set to false if something isn't found
set(GLFW_FOUND TRUE)

if(${GLFW_INCLUDE_DIR-NOTFOUND})
	set(GLFW_FOUND FALSE)
	message(FATAL_ERROR "GLFW/glfw3.h not found.")
endif()

if(${GLFW_LIBRARY-NOTFOUND})
	set(GLFW_FOUND FALSE)
	message(FATAL_ERROR "GLFW lib not found.")
endif()

if(${GLFW_FOUND})
	message(STATUS "GLFW found in ${GLFW_INCLUDE_DIR}")
endif()
