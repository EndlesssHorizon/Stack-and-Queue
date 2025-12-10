cmake_minimum_required(VERSION 3.14)

project(ContainerTest)
set(CMAKE_TLS_VERIFY OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-g -Wall -Wextra -Werror -pedantic)
endif()

include(FetchContent)

message(STATUS "Downloading GoogleTest...")
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_app test_main.cpp)

target_link_libraries(test_app PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_app)