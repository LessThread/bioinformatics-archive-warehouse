cmake_minimum_required(VERSION 3.15)

project(sequencing-preprocessing2)

SET(SRC_LIST ../src/main.cpp ../src/IO/IO.cpp ../src/exception-handling/exception-handling.cpp ../src/class/sequencing/sequencing.cpp)

# add_library(sequencing-preprocessing ${src})  
# set(THREADS_PREFER_PTHREAD_FLAG ON)
# find_package(Threads REQUIRED)
# target_link_libraries(sequencing-preprocessing PUBLIC Threads::Threads)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

set(CMAKE_CXX_STANDARD 17)


add_executable(${PROJECT_NAME} ${SRC_LIST})