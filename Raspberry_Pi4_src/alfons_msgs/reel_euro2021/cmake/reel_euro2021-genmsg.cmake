# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "reel_euro2021: 6 messages, 2 services")

set(MSG_I_FLAGS "-Ireel_euro2021:/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(reel_euro2021_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg" ""
)

get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg" ""
)

get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg" ""
)

get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg" ""
)

get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg" "reel_euro2021/c"
)

get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg" ""
)

get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv" ""
)

get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv" NAME_WE)
add_custom_target(_reel_euro2021_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "reel_euro2021" "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv" "reel_euro2021/c"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)

### Generating Services
_generate_srv_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)
_generate_srv_cpp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
)

### Generating Module File
_generate_module_cpp(reel_euro2021
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(reel_euro2021_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(reel_euro2021_generate_messages reel_euro2021_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_cpp _reel_euro2021_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(reel_euro2021_gencpp)
add_dependencies(reel_euro2021_gencpp reel_euro2021_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS reel_euro2021_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)
_generate_msg_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)
_generate_msg_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)
_generate_msg_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)
_generate_msg_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)
_generate_msg_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)

### Generating Services
_generate_srv_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)
_generate_srv_eus(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
)

### Generating Module File
_generate_module_eus(reel_euro2021
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(reel_euro2021_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(reel_euro2021_generate_messages reel_euro2021_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_eus _reel_euro2021_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(reel_euro2021_geneus)
add_dependencies(reel_euro2021_geneus reel_euro2021_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS reel_euro2021_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)
_generate_msg_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)

### Generating Services
_generate_srv_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)
_generate_srv_lisp(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
)

### Generating Module File
_generate_module_lisp(reel_euro2021
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(reel_euro2021_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(reel_euro2021_generate_messages reel_euro2021_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_lisp _reel_euro2021_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(reel_euro2021_genlisp)
add_dependencies(reel_euro2021_genlisp reel_euro2021_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS reel_euro2021_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)
_generate_msg_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)
_generate_msg_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)
_generate_msg_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)
_generate_msg_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)
_generate_msg_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)

### Generating Services
_generate_srv_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)
_generate_srv_nodejs(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
)

### Generating Module File
_generate_module_nodejs(reel_euro2021
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(reel_euro2021_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(reel_euro2021_generate_messages reel_euro2021_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_nodejs _reel_euro2021_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(reel_euro2021_gennodejs)
add_dependencies(reel_euro2021_gennodejs reel_euro2021_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS reel_euro2021_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)
_generate_msg_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)
_generate_msg_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)
_generate_msg_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)
_generate_msg_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)
_generate_msg_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)

### Generating Services
_generate_srv_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)
_generate_srv_py(reel_euro2021
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv"
  "${MSG_I_FLAGS}"
  "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
)

### Generating Module File
_generate_module_py(reel_euro2021
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(reel_euro2021_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(reel_euro2021_generate_messages reel_euro2021_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/ta7wil.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/goal.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/mission.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/c.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/jdid.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/msg/IntList.msg" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/map.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/srv/jd.srv" NAME_WE)
add_dependencies(reel_euro2021_generate_messages_py _reel_euro2021_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(reel_euro2021_genpy)
add_dependencies(reel_euro2021_genpy reel_euro2021_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS reel_euro2021_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/reel_euro2021
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(reel_euro2021_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/reel_euro2021
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(reel_euro2021_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/reel_euro2021
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(reel_euro2021_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/reel_euro2021
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(reel_euro2021_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/reel_euro2021
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(reel_euro2021_generate_messages_py std_msgs_generate_messages_py)
endif()
