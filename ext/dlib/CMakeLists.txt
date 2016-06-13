cmake_minimum_required(VERSION 2.8.8)
project(dlib_build C CXX)
include(ExternalProject)

ExternalProject_Add(dlib
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    URL http://dlib.net/files/dlib-18.18.tar.bz2
    CONFIGURE_COMMAND cmake ../dlib/dlib
    BUILD_COMMAND cmake --build . --config Release
# Disable install step
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(dlib source_dir)
set(DLIB_INCLUDE_DIRS ${source_dir} PARENT_SCOPE)

# Specify MainTest's link libraries
ExternalProject_Get_Property(dlib binary_dir)
set(DLIB_LIBS_DIR ${binary_dir} PARENT_SCOPE)
