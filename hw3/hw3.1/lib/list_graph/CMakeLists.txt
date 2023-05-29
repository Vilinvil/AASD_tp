add_library(list_graph src/list_graph.cpp)

target_include_directories(list_graph PUBLIC include)

target_link_libraries(list_graph PRIVATE igraph)

target_include_directories(list_graph PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/igraph/include")