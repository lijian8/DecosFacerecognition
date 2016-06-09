cmake_minimum_required(VERSION 2.8.3)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  sensor_msgs
  cv_bridge
  roscpp
  std_msgs
  image_transport
)

include(../../CMakeLists.txt)
project(decos_openface)

###################################
## catkin specific configuration ##
###################################

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES decos_humandetect
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a cpp executable
add_executable(decos_openface src/main.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(decos_openface
   ${catkin_LIBRARIES}
   cpp_openface
)
