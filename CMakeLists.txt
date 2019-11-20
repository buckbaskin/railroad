cmake_minimum_required(VERSION 3.8)

project(railroad VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wreturn-type)
add_compile_options(-Werror=return-type)

include_directories(include/)

add_library(vector_lib
    src/railroad/vector.cpp
)

add_executable(demonstrator src/railroad/main.cpp)

target_link_libraries(demonstrator
    vector_lib
)

install(TARGETS vector_lib DESTINATION lib)
install(TARGETS demonstrator DESTINATION bin)
