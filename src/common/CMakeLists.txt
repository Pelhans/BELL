file(GLOB LIB_SOURCES
    "word_seg_model/*.cpp"
    "util/*.cpp"
)

set(LIB_LINK_LIBRARIES
    -lpthread
    -lxml2
)

add_library(common_lib SHARED ${LIB_SOURCES})
target_link_libraries(common_lib ${LIB_LINK_LIBRARIES})