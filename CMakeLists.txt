cmake_minimum_required(VERSION 3.10)

include(CMakePackageConfigHelpers)

project(StatusOptional LANGUAGES CXX)

set(LIB_NAME statusoptional)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TEST "Build the test suits for the library" OFF)

set(STATUS_OPTIONAL_NAME "statusoptional")
set(STATUS_OPTIONAL_EXPORTNAME "statusoptionalTargets")

set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})

#make the status optional header available when built as a module
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(statusoptional INTERFACE)

add_library(statusoptional::statusoptional ALIAS statusoptional)

target_sources(statusoptional PRIVATE status_optional.h)

target_include_directories(
          statusoptional
	  INTERFACE
	  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
	  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${LIB_NAME}/>
)

get_target_property(LIB_SRC statusoptional SOURCES)

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
  FIND_PACKAGE_ARGS NAMES GTest
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

install(TARGETS statusoptional
          EXPORT ${STATUS_OPTIONAL_EXPORTNAME}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      )


install(
  EXPORT ${STATUS_OPTIONAL_EXPORTNAME}
  DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/cmake/${STATUS_OPTIONAL_NAME}
  NAMESPACE ${STATUS_OPTIONAL_NAME}::
  FILE ${STATUS_OPTIONAL_NAME}Targets.cmake
  )

configure_package_config_file(
  "cmake/Config.cmake.in"
  "${STATUS_OPTIONAL_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/cmake/${STATUS_OPTIONAL_NAME}
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
  )

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${STATUS_OPTIONAL_NAME}ConfigVersion.cmake"
  VERSION "0.0"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${STATUS_OPTIONAL_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${STATUS_OPTIONAL_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/cmake/${STATUS_OPTIONAL_NAME} )
