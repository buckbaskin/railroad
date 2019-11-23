cmake_minimum_required(VERSION 3.8)

project(railroad VERSION 0.1.0)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-O0 -g)
add_compile_options(-Wall -Wextra -Wreturn-type)
add_compile_options(-Werror=return-type)

include_directories(include/)

add_executable(demonstrator example/demo.cpp)

install(TARGETS demonstrator DESTINATION bin)

if(BUILD_TESTING)
    # ... Make Tests ...
endif()
