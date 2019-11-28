cmake_minimum_required(VERSION 3.8)

project(railroad LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-O0 -g)
add_compile_options(-Wall -Wextra -Wreturn-type)
add_compile_options(-Werror)

include_directories(include/)

add_executable(demonstrator example/demo.cpp)

install(TARGETS demonstrator DESTINATION bin)

include(CTest)

if(BUILD_TESTING)
    # find_package(Catch2 REQUIRED)
    # include(Catch)

    include_directories(third_party/Catch2/single_include/)
    add_subdirectory(third_party/Catch2)
    include(third_party/Catch2/contrib/Catch.cmake)
    include(third_party/Catch2/contrib/ParseAndAddCatchTests.cmake)

    add_subdirectory(third_party/rapidcheck)

    file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/railroad/*.cpp")

    add_custom_target(all-tests)

    # ... Make Tests ...
    add_executable(UnitTests ${TEST_FILES})
    target_link_libraries(UnitTests Catch2::Catch2 rapidcheck)
    # catch_discover_tests(UnitTests)
    add_dependencies(all-tests UnitTests)
    add_test(NAME UnitTests COMMAND UnitTests)
endif()
