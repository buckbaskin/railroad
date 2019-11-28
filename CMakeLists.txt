cmake_minimum_required(VERSION 3.8)

project(railroad VERSION 0.1.0)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-O0 -g)
add_compile_options(-Wall -Wextra -Wreturn-type)
add_compile_options(-Werror)

include_directories(include/)

add_executable(demonstrator example/demo.cpp)

install(TARGETS demonstrator DESTINATION bin)

if(BUILD_TESTING)
    include_directories(third_party/Catch2/single_include/)

    add_custom_target(all-tests)

    # ... Make Tests ...
    add_executable(BasicTest test/railroad/basic.cpp)
    add_dependencies(all-tests BasicTest)
    add_test(NAME BasicTest COMMAND BasicTest)
endif()
