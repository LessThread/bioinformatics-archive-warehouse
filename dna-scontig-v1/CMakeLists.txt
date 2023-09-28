cmake_minimum_required(VERSION 3.16)
project(dna-scontig)

add_compile_options(/std:c++17)

include_directories(${PROJECT_SOURCE_DIR}/include)
include(cmake/fmt-8.1.1.cmake)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)