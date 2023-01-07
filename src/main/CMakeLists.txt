add_executable(better_entity_link_server main.cpp entity_link_server.cpp resource_init.cpp)

TARGET_LINK_LIBRARIES(
   better_entity_link_server
   muduo_net
   muduo_base
   pthread
   rt
   plugins_lib
   common_lib
   json_cpp
   clucene-core
   clucene-shared
)