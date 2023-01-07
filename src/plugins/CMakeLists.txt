file(GLOB LIB_SOURCES
    "word_seg/*.cpp"
    "recall/*.cpp"
)

ADD_LIBRARY(plugins_lib SHARED ${LIB_SOURCES})

TARGET_LINK_LIBRARIES(
    plugins_lib
    PUBLIC
    common_lib
)