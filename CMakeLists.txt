cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(sql VERSION 3.15.1 LANGUAGES C CXX)

# Common
include(../compat/compat.cmake)

# Packages
find_package(date REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/../date")
find_package(gsl REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/../gsl")

# Sources
file(GLOB_RECURSE headers include/*.h)
file(GLOB sources src/*.h src/*.cpp)
file(GLOB backend src/sqlite/*.h src/sqlite/*.c)

if(NOT MSVC)
  set(libraries dl)
endif()

# Library
assign_source_group(${headers} ${sources} ${backend})
add_library(${PROJECT_NAME} STATIC ${headers} ${sources} ${backend})
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src src/sqlite)
target_link_libraries(${PROJECT_NAME} PUBLIC date gsl ${libraries})

# To further improve performance, remove SQLITE_ENABLE_COLUMN_METADATA and add SQLITE_OMIT_DECLTYPE.
target_compile_definitions(${PROJECT_NAME} PRIVATE
  SQLITE_THREADSAFE=2
  SQLITE_DEFAULT_MEMSTATUS=1
  SQLITE_LIKE_DOESNT_MATCH_BLOBS
  SQLITE_OMIT_DEPRECATED
  SQLITE_OMIT_PROGRESS_CALLBACK
  SQLITE_OMIT_SHARED_CACHE
  SQLITE_OMIT_WAL
  SQLITE_ENABLE_COLUMN_METADATA)

# Install
if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  if(MSVC)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib/$<LOWER_CASE:$<CONFIG>>)
  else()
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
  endif()
endif()

# Format
if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  add_custom_target(format
    COMMAND clang-format -i ${headers} ${sources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
