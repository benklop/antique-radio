cmake_minimum_required (VERSION 2.6)
project (vfd-display)

# The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/Config.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories(
    "${PROJECT_BINARY_DIR}"
    "driver"
    "src"
    )
 
add_executable(vfd-display src/main.cpp)
