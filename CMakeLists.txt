cmake_minimum_required(VERSION 3.16)
project(cstringpp)
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/cstringpp/core.hpp>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/cstringpp/helpers.hpp>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/cstringpp.hpp>")
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

option(CSTRINGPP_BUILD_TESTS "Build test program for CStringPP" OFF)
if(CSTRINGPP_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1)
    FetchContent_MakeAvailable(googletest)
    enable_testing()

    add_executable(${PROJECT_NAME}_test test/test.cpp)
    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} gtest_main)
    target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_test)

    # Copy gtest binaries next to the executable
    add_custom_command(
            TARGET ${PROJECT_NAME}_test
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/bin ${CMAKE_BINARY_DIR})
endif()
