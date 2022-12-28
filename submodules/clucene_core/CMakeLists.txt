PROJECT (clucene)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.2 FATAL_ERROR)

#define options...

Include (CLuceneDocs)
Include (FindThreads)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ELSE(NOT CMAKE_BUILD_TYPE)
    MESSAGE( "Compiling as ${CMAKE_BUILD_TYPE}" )
ENDIF(NOT CMAKE_BUILD_TYPE)

OPTION(ENABLE_DEBUG
  "enable debug support"
  OFF)
OPTION(ENABLE_MMAP
  "enable mmap support (experimental)"
  OFF)
OPTION(DISABLE_MULTITHREADING
  "disable multithreading - remove all locking code"
  OFF)
OPTION(ENABLE_DMALLOC
  "enable dmalloc memory leak checker"
  OFF)
OPTION(ENABLE_ASCII_MODE
  "enable ascii support"
  OFF)
OPTION(LUCENE_USE_INTERNAL_CHAR_FUNCTIONS
  "use internal character functions. required to run tests correctly"
  ON)
OPTION(ENABLE_PACKAGING
  "create build scripts for creating clucene packages"
  OFF)
OPTION(BUILD_STATIC_LIBRARIES
  "create targets for building static libraries"
  OFF)
OPTION(BUILD_CONTRIBS
  "create targets for building the clucene-contribs"
  OFF)
OPTION(BUILD_CONTRIBS_LIB
  "create targets for building the clucene-contribs-lib"
  OFF)
SET(LUCENE_SYS_INCLUDES "" CACHE PATH
      "location for non-system independent files. defaults to CMAKE_INSTALL_PREFIX. see INSTALL documentation for further information."
      )
#install path options
SET(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
SET(LIB_DESTINATION "lib${LIB_SUFFIX}")

#use single output directory
INCLUDE_DIRECTORIES( ${clucene_SOURCE_DIR}/src/shared )
INCLUDE_DIRECTORIES( ${clucene_BINARY_DIR}/src/shared )
INCLUDE_DIRECTORIES( ${clucene_SOURCE_DIR}/src/core )

#include the projects
ADD_SUBDIRECTORY (src/ext)
ADD_SUBDIRECTORY (src/shared)
ADD_SUBDIRECTORY (src/core)
