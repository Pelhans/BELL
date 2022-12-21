file(GLOB LIB_SOURCES
    "util/*.cpp"
    "config/*.cpp"
    "model/*.cpp"
    "model/word_seg_model/*.cpp"
)

set(LIB_LINK_LIBRARIES
    -lpthread
    -lxml2
)

add_library(common_lib SHARED ${LIB_SOURCES})
target_link_libraries(common_lib ${LIB_LINK_LIBRARIES})