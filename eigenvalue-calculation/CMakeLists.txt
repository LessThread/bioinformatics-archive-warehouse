cmake_minimum_required(VERSION 2.8)
project(EIGENVALUE_MATRIX_GENERATION)

# include_directories(./include/)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
aux_source_directory(./src/ SOURCE_DIR)
aux_source_directory(.. SOURCE_DIR)

add_executable(test ${SOURCE_DIR} ../main.cpp)
