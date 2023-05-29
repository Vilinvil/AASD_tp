add_library(matrix_graph src/matrix_graph.cpp)

target_include_directories(matrix_graph PUBLIC include)

target_link_libraries(matrix_graph PRIVATE igraph)

target_include_directories(matrix_graph PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/igraph/include")