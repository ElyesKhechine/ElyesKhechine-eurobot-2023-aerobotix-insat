cmake_minimum_required(VERSION 3.0.2)
project(reel_euro2021)



find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy

  # others
  std_msgs
  message_generation
  tf
)

add_message_files(
   FILES
   ta7wil.msg
   goal.msg
   mission.msg
   c.msg
   jdid.msg
   IntList.msg

#   Message2.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  map.srv
  jd.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
 )
 


catkin_package(
  INCLUDE_DIRS
#  LIBRARIES reel_euro2021
  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
CATKIN_DEPENDS message_runtime  geometry_msgs std_msgs
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include ${catkin_INCLUDE_DIRS})

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
catkin_install_python(PROGRAMS
  scripts/missions_sub.py
  scripts/obstacle.py
  scripts/screenReciever.py
  scripts/blasetdisques.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_reel_euro2021.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()
add_executable(ta7wil src/ta7wil.cpp)
add_dependencies(ta7wil ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(ta7wil ${catkin_LIBRARIES}
)
add_executable(goal src/goal.cpp)
add_dependencies(goal ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(goal ${catkin_LIBRARIES}
)
add_executable(evitement src/evitement.cpp)
add_dependencies(evitement ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(evitement ${catkin_LIBRARIES}
)

add_executable(sequence src/sequence.cpp)
add_dependencies(sequence ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(sequence ${catkin_LIBRARIES}
)

add_executable(mapupdate src/mapupdate.cpp)
add_dependencies(mapupdate ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(mapupdate ${catkin_LIBRARIES}
)
add_executable(scoreEstimation src/scoreEstimation.cpp)
add_dependencies(scoreEstimation ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(scoreEstimation ${catkin_LIBRARIES}
)

add_executable(detectionArucoMilieu src/detectionArucoMilieu.cpp)
add_dependencies(detectionArucoMilieu ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(detectionArucoMilieu ${catkin_LIBRARIES}
)

#target_link_libraries(send_goal ${catkin_LIBRARIES}
#)


## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
