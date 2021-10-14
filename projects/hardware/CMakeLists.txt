project(Hardware_threads_etc)
cmake_minimum_required(VERSION 3.18)

ADD_EXECUTABLE(Hardware_threads_etc main.cc)
target_link_libraries(Hardware_threads_etc pthread)
