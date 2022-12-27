file(GLOB LIB_SOURCES
    "analysis/*.cpp"
    "analysis/standard/*.cpp"
    "debug/*.cpp"
    "document/*.cpp"
    "index/*.cpp"
    "queryParser/*.cpp"
    "queryParser/legacy/*.cpp"
    "search/*.cpp"
    "search/spans/*.cpp"
    "store/*.cpp"
    "util/*.cpp"
    "*.cpp"
)

set(LIB_LINK_LIBRARIES
    -lpthread
)

add_library(clucene_lib SHARED ${LIB_SOURCES})
target_link_libraries(clucene_lib ${LIB_LINK_LIBRARIES})

set(HEADERS
    "*.h"
    "analysis/*.h"
    "analysis/standard/*.h"
    "debug/*.h"
    "document/*.h"
    "index/*.h"
    "queryParser/*.h"
    "queryParser/legacy/*.h"
    "search/*.h"
    "search/spans/*.h"
    "store/*.h"
    "util/*.h"
  )
install(FILES ${HEADERS} DESTINATION include/clucene_core)
