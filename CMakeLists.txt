cmake_minimum_required(VERSION 3.0)

# Define project
project(bell CXX)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/submodules/clucene_core/cmake" "${CMAKE_SOURCE_DIR}/submodules/clucene_core/src/shared/cmake")

ADD_DEFINITIONS(
  -DISFULLINDEX
)

##############################
set(BELL_ROOT "${PROJECT_SOURCE_DIR}/src")
set(SUBMODULES_PATH "${PROJECT_SOURCE_DIR}/submodules")

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -ggdb -og -fPIC -w -Wl,--export-dynamic -Wno-deprecated -fpermissive")
elseif(CMAKE_BUILD_TYPE STREQUAL "Noop")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g -o0 -fPIC -w -Wl,--export-dynamic -Wno-deprecated -fpermissive")
else()
  set(CMAKE_CXX_FLAGS "-lstdc++ -std=c++11 ${CMAKE_CXX_FLAGS} -g -O3 -fPIC -w -Wl,--export-dynamic -Wno-dev -Wno-deprecated -fpermissive")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

message("=================================")
message("Build Type:==${CMAKE_BUILD_TYPE}==")
message("CXX_FLAGS:==${CMAKE_CXX_FLAGS}==")
message("=================================")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(
  "${SUBMODULES_PATH}"
  "${BELL_ROOT}"
  "${BELL_ROOT}/common"
  "${BELL_ROOT}/common/data"
  "${BELL_ROOT}/common/data/record"
  "${BELL_ROOT}/common/config"
  "${BELL_ROOT}/common/eigen"
  "${BELL_ROOT}/common/model"
  "${BELL_ROOT}/common/model/word_seg_model"
  "${BELL_ROOT}/plugins"
  "${BELL_ROOT}/plugins/word_seg"
  "${BELL_ROOT}/plugins/recall"
  "${SUBMODULES_PATH}/clucene_core"
)

find_library(clucene-core_lib NAMES clucene-core PATHS ./submodules/clucene_core/clucene-core)
find_library(clucene-shared_lib NAMES clucene-shared PATHS ./submodules/clucene_core/clucene-shared)


 link_directories(
  ${SUBMODULES_PATH}/muduo
  ${SUBMODULES_PATH}/muduo/base
  ${SUBMODULES_PATH}/muduo/net
  ${SUBMODULES_PATH}/json_cpp
  ${SUBMODULES_PATH}/clucene_core
  ${SUBMODULES_PATH}/clucene_core/analysis
  ${SUBMODULES_PATH}/clucene_core/analysis/standard
 )

add_subdirectory(${SUBMODULES_PATH}/muduo/base)
add_subdirectory(${SUBMODULES_PATH}/muduo/net)
add_subdirectory(${SUBMODULES_PATH}/json_cpp)
add_subdirectory(${SUBMODULES_PATH}/clucene_core)
add_subdirectory(${BELL_ROOT}/common common)
add_subdirectory(${BELL_ROOT}/plugins plugins)
add_subdirectory(${BELL_ROOT}/main main)
