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
   TrexRobotData.msg
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
add_dependencies(pi_trex_pwr_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_pwr_subscriber ${catkin_LIBRARIES})

add_executable(pi_trex_accel_subscriber src/pi_trex_accel_subscriber.cpp)
add_dependencies(pi_trex_accel_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_accel_subscriber ${catkin_LIBRARIES})

add_executable(pi_trex_irange_subscriber src/pi_trex_irange_subscriber.cpp)
add_dependencies(pi_trex_irange_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_irange_subscriber ${catkin_LIBRARIES})

add_executable(pi_trex_ultrasonic_subscriber src/pi_trex_ultrasonic_subscriber.cpp)
add_dependencies(pi_trex_ultrasonic_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_ultrasonic_subscriber ${catkin_LIBRARIES})

install(PROGRAMS
  scripts/pi_trex_publisher.py 
  scripts/pi_trex_Pwr_subscriber.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

 install(TARGETS pi_trex_pwr_subscriber pi_trex_accel_subscriber pi_trex_irange_subscriber pi_trex_ultrasonic_subscriber
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
