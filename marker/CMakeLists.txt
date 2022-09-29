cmake_minimum_required(VERSION 3.0.2)
project(marker)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  tf
  #darknet_ros  
  darknet_ros_msgs
  )


catkin_package(
  CATKIN_DEPENDS
  INCLUDE_DIRS
  LIBRARIES
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
add_executable(marker_human src/marker_human.cpp)
target_link_libraries(marker_human ${catkin_LIBRARIES})
add_executable(platform_marker src/platform_marker.cpp)
target_link_libraries(platform_marker ${catkin_LIBRARIES})