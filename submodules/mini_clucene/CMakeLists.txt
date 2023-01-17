PROJECT (mini_clucene)

#Rules for version:
#MAJOR and MINOR versions are purely political (tracks JLucene compatibility)
#REVISION version MUST be revised if the headers or compatibility change
#PATCH should be 0 unless a patch is made that doesn't affect the public signature (i.e. clients don't need to re-compile).
SET(CLUCENE_VERSION_MAJOR "2")
SET(CLUCENE_VERSION_MINOR "3")
SET(CLUCENE_VERSION_REVISION "3")
SET(CLUCENE_VERSION_PATCH "4")

# SOVERSION information
#Must be incremented for releases if the api is not backwards compatible
SET(CLUCENE_SOVERSION "1")

MATH(EXPR CLUCENE_INT_VERSION "(${CLUCENE_VERSION_MAJOR} * 1000000) + (${CLUCENE_VERSION_MINOR} * 10000) + (${CLUCENE_VERSION_REVISION} * 100) + (${CLUCENE_VERSION_PATCH} * 1)" )
SET(CLUCENE_VERSION "${CLUCENE_VERSION_MAJOR}.${CLUCENE_VERSION_MINOR}.${CLUCENE_VERSION_REVISION}.${CLUCENE_VERSION_PATCH}")

#CMake 2.6+ is recommended to an improved Boost module
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0 FATAL_ERROR)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#now write out our configuration....
CONFIGURE_FILE(${mini_clucene_SOURCE_DIR}/shared/clucene-config.h.cmake ${mini_clucene_BINARY_DIR}/shared/clucene-config.h)
CONFIGURE_FILE(${mini_clucene_SOURCE_DIR}/shared/_clucene-config.h.cmake ${mini_clucene_BINARY_DIR}/shared/_clucene-config.h)

#ADD_SUBDIRECTORY (src/shared)

file(GLOB LIB_SOURCES
    "analysis/*.cpp"
    "debug/*.cpp"
    "util/*.cpp"
    "index/*.cpp"
    "shared/*.cpp"
    "config/*.cpp"
)

include_directories(
    "analysis"
    "debug"
    "util"
    "index"
    "shared"
    "config"
)

set(base_SRCS
    analysis/AnalysisHeader.cpp
    debug/condition.cpp
    debug/error.cpp
    index/Payload.cpp
    shared/StringBuffer.cpp
)

add_library(mini_clucene ${base_SRCS})
target_link_libraries(mini_clucene pthread rt)

install(TARGETS mini_clucene DESTINATION lib)

file(GLOB HEADERS "*.h" "shared/*.h" "util/*.h" "analysis/*.h" "config/*.h" "debug/*.h" "index/*.h")
install(FILES ${HEADERS} DESTINATION include/mini_clucene)