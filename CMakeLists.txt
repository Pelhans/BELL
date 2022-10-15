cmake_minimum_required(VERSION 3.0)

# Define project
project(bell CXX)

ADD_DEFINITIONS(
  -DISFULLINDEX
)
##############################
set(BELL_ROOT "${PROJECT_SOURCE_DIR}/src")

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -ggdb -og -fPIC -w -Wl,--export-dynamic -Wno-deprecated -fpermissive")
elseif(CMAKE_BUILD_TYPE STREQUAL "Noop")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -o0 -fPIC -w -Wl,--export-dynamic -Wno-deprecated -fpermissive")
else()
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -o3 -fPIC -w -Wl,--export-dynamic -Wno-deprecated -fpermissive")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

message("=================================")
message("Build Type:==${CMAKE_BUILD_TYPE}==")
message("CXX_FLAGS:==${CMAKE_CXX_FLAGS}==")
message("=================================")


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories(
  "${BELL_ROOT}"
  "${BELL_ROOT}/plugins"
  "${BELL_ROOT}/plugins/word_seg"
  # "${BELL_ROOT}/common"
)

# link_directories(
#   "${BELL_ROOT}/common"
#   "${BELL_ROOT}/common/xgboost"
#   "${BELL_ROOT}/common/eigen"
# )

# add_subdirectory(${BELL_ROOT}/common common)
add_subdirectory(${BELL_ROOT}/plugins plugins)
add_subdirectory(${BELL_ROOT}/main main)