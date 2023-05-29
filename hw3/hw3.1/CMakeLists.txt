cmake_minimum_required(VERSION 3.20)
project(hw3.1 VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

add_subdirectory(lib/list_graph)
add_subdirectory(lib/matrix_graph)
add_subdirectory(lib/set_graph)
add_subdirectory(lib/arc_graph)
add_subdirectory(lib/igraph)

add_executable(hw3.1 src/main.cpp)
target_link_libraries(hw3.1 igraph list_graph matrix_graph set_graph arc_graph)

add_custom_target(valgrind
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
    --verbose $<TARGET_FILE:hw3.1>
    DEPENDS hw3.1
    COMMENT "Running Valgrind")
