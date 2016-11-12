cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR xxcalc)
project(${PROJECT_NAME_STR} CXX)

set(CXX_CLANG_FLAGS "-Wall -Wextra -Wno-unused-parameter -std=c++11 -stdlib=libc++ -O3 -march=native")
set(CXX_GNU_FLAGS "-std=c++11 -Wall -O3 -march=native")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CLANG_FLAGS}")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_GNU_FLAGS}")
endif()

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")

set(EXT_PROJECTS_DIR "${PROJECT_SOURCE_DIR}/ext")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(COMMON_INCLUDES "${SOURCE_DIR}")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")

add_subdirectory("${SOURCE_DIR}/apps")

