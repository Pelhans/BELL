file(GLOB LIB_SOURCES
    "word_seg_model/*.cpp"
    "util/*.cpp"
)

set(LIB_LINK_LIBRARIES
    -lpthread
)

add_library(common_lib STATIC ${LIB_SOURCES})
target_link_libraries(common_lib ${LIB_LINK_LIBRARIES}
)