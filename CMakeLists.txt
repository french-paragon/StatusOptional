cmake_minimum_required(VERSION 3.10)

project(StatusOptional LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TEST "Build the test suits for the library" OFF)

#make the status optional header available when built as a module
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (BUILD_TEST)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  status_optional_test
  status_optional.h
  test.cpp
)
target_link_libraries(
  status_optional_test
  GTest::gtest_main
)

endif()
