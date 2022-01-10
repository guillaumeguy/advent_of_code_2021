cmake_minimum_required(VERSION 3.20)
project(aoc2021)

set(CMAKE_CXX_STANDARD 17)

add_executable(aoc2021 main.cpp)

# add_compile_options("-fopenmp -Wshadow -Wall -march=native -fsanitize=address")
# SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(aoc2021 PUBLIC OpenMP::OpenMP_CXX)
endif()

