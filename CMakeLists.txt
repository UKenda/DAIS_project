cmake_minimum_required(VERSION 3.0.2)
project(simulation)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  gazebo_ros
)
find_package(gazebo REQUIRED)

catkin_package(
  DEPENDS
    gazebo_ros
  CATKIN_DEPENDS
  INCLUDE_DIRS
  LIBRARIES
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS} 
  ${SDFormat_INCLUDE_DIRS}
)
add_executable(marker src/marker.cpp)
target_link_libraries(marker ${catkin_LIBRARIES})
add_executable(platform_marker src/platform_marker.cpp)
target_link_libraries(platform_marker ${catkin_LIBRARIES})