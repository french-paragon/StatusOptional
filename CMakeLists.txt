cmake_minimum_required(VERSION 3.10)

project(StatusOptional LANGUAGES CXX)

set(LIB_NAME statusoptional)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TEST "Build the test suits for the library" OFF)

#make the status optional header available when built as a module
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(statusoptional-lib INTERFACE)
target_include_directories(statusoptional-lib INTERFACE .)

add_library(statusoptional::statusoptional ALIAS statusoptional-lib)

target_sources(statusoptional-lib PRIVATE status_optional.h)

target_include_directories(
	  statusoptional-lib
	  INTERFACE
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
	  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}/>
)

get_target_property(LIB_SRC statusoptional-lib SOURCES)

foreach(FILE IN LISTS LIB_SRC)
	    if(FILE MATCHES "^((.*)/)?([^/]+)\\.h$")
			message("Configuring file ${FILE} for installation in ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}/${CMAKE_MATCH_2}")
			install(FILES ${FILE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}/${CMAKE_MATCH_2})
		endif()
endforeach()

if (BUILD_TEST)

include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP ON
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
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
