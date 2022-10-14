cmake_minimum_required(VERSION 3.0)

project(bell CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

add_subdirectory(muduo)
include_directories("muduo")



add_executable(bell ../src/entity_link_server.cpp)
target_link_libraries(bell muduo_net)
target_link_libraries(bell muduo_base)
target_link_libraries(bell pthread rt)
