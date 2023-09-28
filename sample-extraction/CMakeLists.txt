cmake_minimum_required(VERSION 3.15)
project(sample-extraction)

# 这里可以添加操作系统环境判断，动态寻找 boost库
set(BOOST_ROOT E:/lab/cpp/boost/boost_1_80_0)
set(BOOST_ROOT E:/lab/cpp/boost/boost_1_80_0)
set(Boost_LIBRARY_DIRS  E:/lab/cpp/boost/boost_1_80_0/stage/lib)

find_package(boost)


link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})


# message(Boost\ version: ${Boost_VERSION_STRING})
# message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
# message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})
# foreach(boost_lib IN LISTS Boost_LIBRARIES)
#     message(${boost_lib})
#     string(REGEX MATCH ".+/.*boost_([^-]+)-.+\.(lib|a)" boost_lib_name ${boost_lib})
#     set(boost_lib_name ${CMAKE_MATCH_1})
#     set(boost_target Boost::${boost_lib_name})
#     if(TARGET ${boost_target})
#         message(STATUS "Boost target found: " ${boost_target})
#     endif(TARGET ${boost_target})
# endforeach(boost_lib)

message(Boost\ version: ${Boost_VERSION_STRING})
message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})


# file(GLOB_RECURSE ALL_SOURCE "*.cpp")

# aux_source_directory(. sources)
# set(sources src/main.cpp src/program_options/program_options.cpp)

aux_source_directory(src sources)
include_directories (include includes)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PUBLIC ${sources} ${includes})
