# Install script for directory: /home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/reel_euro2021/msg" TYPE FILE FILES
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg"
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg"
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg"
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg"
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/reel_euro2021/srv" TYPE FILE FILES
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv"
    "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/reel_euro2021/cmake" TYPE FILE FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/reel_euro2021-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/devel/include/reel_euro2021")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/devel/share/roseus/ros/reel_euro2021")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/devel/share/common-lisp/ros/reel_euro2021")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/devel/share/gennodejs/ros/reel_euro2021")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python3" -m compileall "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/devel/lib/python3/dist-packages/reel_euro2021")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3/dist-packages" TYPE DIRECTORY FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/devel/lib/python3/dist-packages/reel_euro2021")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/reel_euro2021.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/reel_euro2021/cmake" TYPE FILE FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/reel_euro2021-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/reel_euro2021/cmake" TYPE FILE FILES
    "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/reel_euro2021Config.cmake"
    "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/reel_euro2021Config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/reel_euro2021" TYPE FILE FILES "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/reel_euro2021" TYPE PROGRAM FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/missions_sub.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/reel_euro2021" TYPE PROGRAM FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/obstacle.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/reel_euro2021" TYPE PROGRAM FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/screenReciever.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/reel_euro2021" TYPE PROGRAM FILES "/home/ubuntu/AeroBotiX_euro2021/src/alfons_msgs/reel_euro2021/catkin_generated/installspace/blasetdisques.py")
endif()

