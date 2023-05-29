add_library(set_graph src/set_graph.cpp)

target_include_directories(set_graph PUBLIC include)

target_link_libraries(set_graph PRIVATE igraph)

target_include_directories(set_graph PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/igraph/include")