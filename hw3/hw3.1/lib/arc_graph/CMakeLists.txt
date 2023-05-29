add_library(arc_graph src/arc_graph.cpp)

target_include_directories(arc_graph PUBLIC include)

target_link_libraries(arc_graph PRIVATE igraph)

target_include_directories(arc_graph PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/igraph/include")