cmake_minimum_required(VERSION 2.8.3)
project(dagu_trex_atmega328p)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  rosserial_arduino
  std_msgs
  message_generation
)

 add_message_files(
   FILES
   TrexRobotData.msg
   TrexServoMotorCmds.msg
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

add_executable(pi_trex_vel_subscriber src/pi_trex_vel_subscriber.cpp)
add_dependencies(pi_trex_vel_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_vel_subscriber ${catkin_LIBRARIES})

add_executable(pi_trex_accel_subscriber src/pi_trex_accel_subscriber.cpp)
add_dependencies(pi_trex_accel_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_accel_subscriber ${catkin_LIBRARIES})

add_executable(pi_trex_irange_subscriber src/pi_trex_irange_subscriber.cpp)
add_dependencies(pi_trex_irange_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_irange_subscriber ${catkin_LIBRARIES})

add_executable(pi_trex_encoders_subscriber src/pi_trex_encoders_subscriber.cpp)
add_dependencies(pi_trex_encoders_subscriber ${PROJECTNAME}_gencpp)
target_link_libraries(pi_trex_encoders_subscriber ${catkin_LIBRARIES})

add_executable(pi_trex_rob_loc_subscriber src/pi_trex_rob_loc_subscriber.cpp)
add_dependencies(pi_trex_rob_loc_subscriber ${PROJECT_NAME}_gencpp)
target_link_libraries(pi_trex_rob_loc_subscriber ${catkin_LIBRARIES})

install(PROGRAMS
  scripts/pi_trex_publisher.py 
  scripts/pi_trex_Pwr_subscriber.py
  scripts/pi_trex_Velocity_subscriber.py
  scripts/pi_trex_RobLoc_subscriber.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

 install(TARGETS pi_trex_pwr_subscriber pi_trex_accel_subscriber pi_trex_irange_subscriber pi_trex_vel_subscriber pi_trex_encoders_subscriber pi_trex_rob_loc_subscriber
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
