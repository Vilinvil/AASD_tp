cmake_minimum_required(VERSION 3.20)
project(hw3.2 VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

add_executable(hw3.2 src/main.cpp)
target_link_libraries(hw3.2 list_graph)

add_custom_target(valgrind
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
    --verbose $<TARGET_FILE:hw3.2>
    DEPENDS hw3.2
    COMMENT "Running Valgrind")
