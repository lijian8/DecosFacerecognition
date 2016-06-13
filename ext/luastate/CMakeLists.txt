cmake_minimum_required(VERSION 2.8.8)
project(luastate C CXX)
include(ExternalProject)

add_definitions(-std=gnu++11)

ExternalProject_Add(luastate
    GIT_REPOSITORY https://github.com/AdUki/LuaState.git
    PREFIX "${CMAKE_CURRENT_LIST_DIR}"
    BUILD_COMMAND ""
# Disable install step
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(luastate source_dir)
set(LUASTATE_INCLUDE_DIR ${source_dir}/include)
