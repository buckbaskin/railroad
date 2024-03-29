cmake_minimum_required(VERSION 3.8)

project(railroad LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Og -g)
add_compile_options(-Wall -Wextra -Wreturn-type)
add_compile_options(-Werror)
# add_compile_options(-Wconversion -Wno-error=conversion)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# Added so that rapidcheck can build on Clang-10
# add_compile_options(-Wno-implicit-int-float-conversion)
add_compile_options(-fembed-bitcode -Wunused-lambda-capture)
endif()

set(CHECK_OPTIMIZATIONS FALSE)

if (CHECK_OPTIMIZATIONS)
add_compile_options(-fopt-info-all-optall=all_opt.txt)
endif()

include_directories(include/)

add_executable(demonstrator example/demo.cpp)

install(TARGETS demonstrator DESTINATION bin)

include(CTest)

if(BUILD_TESTING AND (NOT CHECK_OPTIMIZATIONS))
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
