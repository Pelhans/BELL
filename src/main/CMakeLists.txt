add_executable(better_entity_link_server main.cpp entity_link_handler.cpp)

TARGET_LINK_LIBRARIES(
   better_entity_link_server
   PUBLIC
   plugins_lib
)