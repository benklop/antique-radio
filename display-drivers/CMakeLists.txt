cmake_minimum_required (VERSION 3.1)
project (vfd-display)

# The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
 
set (CMAKE_CXX_STANDARD 14)

# do not use an X11 display
set (cimg_display 0)

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
    "${PROJECT_SOURCE_DIR}/driver"
    "${PROJECT_SOURCE_DIR}/src"
    )

# build the gu7000 library so we can actually do things
add_library(gu7000 STATIC

    "driver/config.h"
    "driver/interface.h"
    "driver/Noritake_VFD_GU7000.h"
    "driver/linux_serial.cpp"
    "driver/asyncSerial.cpp"
    "driver/paralleli80.cpp"
    "driver/spi.cpp"
    "driver/syncSerial.cpp"
    "driver/Noritake_VFD_GU7000.cpp"
    )

add_executable(vfd-display src/vfd_writer.cpp src/main.cpp)
target_link_libraries(vfd-display gu7000 pthread)
