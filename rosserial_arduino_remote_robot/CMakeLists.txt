cmake_minimum_required(VERSION 2.8.3)
project(trex_robot_control)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  rosserial_arduino
  std_msgs
  message_generation
)

 add_message_files(
   FILES
   TrexMotorCmds.msg
   TrexPowerMsg.msg
 )

 generate_messages(
   DEPENDENCIES
   std_msgs
 )

catkin_package(
  INCLUDE_DIRS include
  INCLUDE_DIRS src
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp rospy rosserial_arduino std_msgs message_runtime
#  DEPENDS system_lib
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(pi_trex_pwr_subscriber src/pi_trex_pwr_subscriber.cpp)
add_dependencies(pi_trex_pwr_subscriber trex_robot_control_generate_messages_cpp)
target_link_libraries(pi_trex_pwr_subscriber ${catkin_LIBRARIES})

install(PROGRAMS
  scripts/pi_trex_publisher.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

 install(TARGETS pi_trex_pwr_subscriber
   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)

install(DIRECTORY launch/
 DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch 
)
