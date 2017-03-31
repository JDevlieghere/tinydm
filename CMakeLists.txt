cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project (TinyDM)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SANITIZE OFF CACHE BOOL "Build with sanitizers")
if(SANITIZE)
  message(STATUS "Building with sanitizers")
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
else(SANITIZE)
  message(STATUS "Building without sanitizers")
endif(SANITIZE)

# TinyDM Target
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)

add_executable(tinydm ${SOURCE_FILES})
target_link_libraries(tinydm curl pthread)
target_include_directories(tinydm PRIVATE ${HEADER_DIR})

set_property(TARGET tinydm PROPERTY CXX_STANDARD 14)
