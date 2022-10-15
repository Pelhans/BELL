add_executable(better_entity_link_server main.cpp entity_link_server.cpp)

# link_directories(
#     ${MUDUO_PATH}/muduo/muduo
#     ${MUDUO_PATH}/muduo/muduo/base
#     ${MUDUO_PATH}/muduo/muduo/net
# )

TARGET_LINK_LIBRARIES(
   better_entity_link_server
   muduo_net
   muduo_base
   pthread
   rt
   plugins_lib
   common_lib
)