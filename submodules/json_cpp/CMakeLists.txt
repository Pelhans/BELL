set(base_SRCS
json_reader.cpp   
json_value.cpp 
json_writer.cpp
  )

add_library(json_cpp ${base_SRCS})
target_link_libraries(json_cpp pthread rt)

install(TARGETS json_cpp DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/json_cpp)
