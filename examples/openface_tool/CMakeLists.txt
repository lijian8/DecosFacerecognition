cmake_minimum_required(VERSION 2.8.3)

project(openface_tool)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

## Declare a cpp executable
add_executable(openface_tool
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/tool.cpp 
)

## Specify libraries to link a library or executable target against
target_link_libraries(openface_tool
   cpp_openface
   boost_filesystem
   boost_system
   boost_program_options
)
