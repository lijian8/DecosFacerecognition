cmake_minimum_required(VERSION 2.8)
project(cpp_openface)
set(CMAKE_CXX_STANDARD 11)

# Dependencies
find_package(OpenCV 3 REQUIRED)

include(/usr/local/include/dlib/cmake)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -std=gnu++11 -ansi -Wno-deprecated -pthread)
endif()

#-------------------
# set common include folder for module
#-------------------
set(EXT_PROJECTS_DIR ${CMAKE_CURRENT_LIST_DIR}/ext)
message(${CMAKE_CURRENT_LIST_DIR})
# include(${EXT_PROJECTS_DIR}/dlib/CMakeLists.txt)
include(${EXT_PROJECTS_DIR}/luastate/CMakeLists.txt)
# TODO: Change hardcoded torch path
set(COMMON_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include $ENV{HOME}/torch/install/include ${LUASTATE_INCLUDE_DIR} ${DLIB_INCLUDE_DIRS})
set(COMMON_LIBS $ENV{HOME}/torch/install/lib)

#-------------------
# Module source
#-------------------
set(PROJECT_LIB_NAME ${PROJECT_NAME_STR})
link_directories(${COMMON_LIBS} ${DLIB_LIBS_DIR} /opt/X11/lib)  # TODO: Change X11 library path
include_directories(${COMMON_INCLUDES})
file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/**/*.cpp)
add_library(cpp_openface ${SRC_FILES})
add_dependencies(cpp_openface dlib luastate)
target_link_libraries(cpp_openface TH lua5.1 luaT dlib ${OpenCV_LIBS})
install(TARGETS cpp_openface LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

#-------------------
# Examples
#-------------------

add_executable(webcam_example ${CMAKE_CURRENT_LIST_DIR}/examples/webcam.cpp)
target_link_libraries(webcam_example cpp_openface X11)

add_executable(database_builder ${CMAKE_CURRENT_LIST_DIR}/examples/database_builder.cpp)
target_link_libraries(database_builder cpp_openface X11)

add_executable(database_processor ${CMAKE_CURRENT_LIST_DIR}/examples/database_processor.cpp)
target_link_libraries(database_processor cpp_openface boost_filesystem boost_system)

#-------------------
# Benchmarks
#-------------------

add_executable(detection_benchmark ${CMAKE_CURRENT_LIST_DIR}/benchmark/detection.cpp)
target_link_libraries(detection_benchmark cpp_openface X11)

#-------------------
# Documentation
#-------------------

find_package(Doxygen)
if(DOXYGEN_FOUND)
  option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ON) #${DOXYGEN_FOUND})

  if(BUILD_DOCUMENTATION)
      if(NOT DOXYGEN_FOUND)
          message(FATAL_ERROR "Doxygen is needed to build the documentation.")
      endif()
      set(doxyfile ${CMAKE_CURRENT_LIST_DIR}/.doxyconfig)

      add_custom_target(doc ALL
          COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
          WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
          COMMENT "Generating API documentation with Doxygen")
  endif()
endif()

#-------------------
# Test
#-------------------
include(${EXT_PROJECTS_DIR}/gtest/CMakeLists.txt)

macro(_add_test test_name)
  add_executable(${test_name}_test ${CMAKE_CURRENT_LIST_DIR}/test/${test_name}.cpp)
  add_dependencies(${test_name}_test googletest dlib)

  target_link_libraries(${test_name}_test
          cpp_openface
          dlib
          ${GTEST_LIBS_DIR}/libgtest.a
          ${GTEST_LIBS_DIR}/libgtest_main.a
          )
  add_test(${test_name} ${test_name}_test WORKING_DIRECTORY /Users/jan/cpp_openface/)
endmacro(_add_test)

enable_testing()
set(PROJECT_TEST_NAME all_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

# _add_test(core)
add_executable(core_test ${CMAKE_CURRENT_LIST_DIR}/test/core.cpp)
add_dependencies(core_test googletest dlib)

target_link_libraries(core_test
        cpp_openface
        dlib
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        )
_add_test(openface)
_add_test(database)
_add_test(detection)
_add_test(learning)

file(GLOB TEST_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest dlib)

target_link_libraries(${PROJECT_TEST_NAME}
        cpp_openface
        dlib
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        )

add_test(test1 ${PROJECT_TEST_NAME} WORKING_DIRECTORY /Users/jan/cpp_openface/)
