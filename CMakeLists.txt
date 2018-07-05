cmake_minimum_required(VERSION 3.1)
include(ExternalProject)

project(bloomfiltertrie)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Make all code position independent, for shared libraries. Turn off for the highest optimization
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# TARGETS
# bft target holds each object file
add_library(bft OBJECT "")

# the bft executable
add_executable(bft_bin $<TARGET_OBJECTS:bft> ${CMAKE_SOURCE_DIR}/src/main.c)
set_target_properties(bft_bin
  PROPERTIES OUTPUT_NAME bft)
target_include_directories(bft_bin PRIVATE "${CMAKE_SOURCE_DIR}/include")

# the bft static library
add_library(bft_static STATIC $<TARGET_OBJECTS:bft>)

# the bft shared library
add_library(bft_shared SHARED $<TARGET_OBJECTS:bft>)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c99 -msse4.1 -mpopcnt -DXXH_NAMESPACE=BFT_HASH_")

# Set proper include directory
target_include_directories(bft PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Add the source files from the subdirectories
include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/include/CMakeLists.txt)

# Set the proper installation directory
install(TARGETS bft_bin bft_static bft_shared DESTINATION ${CMAKE_INSTALL_PREFIX})

# DEPENDENCIES
# Judy Arrays
set(PROJECT_JUDY_PATH ${CMAKE_BINARY_DIR}/judy-1.0.5)
ExternalProject_Add(project_judy
  URL https://downloads.sourceforge.net/project/judy/judy/Judy-1.0.5/Judy-1.0.5.tar.gz
  URL_HASH MD5=115a0d26302676e962ae2f70ec484a54
  PREFIX ${PROJECT_JUDY_PATH}
  CONFIGURE_COMMAND ${PROJECT_JUDY_PATH}/src/project_judy/configure --prefix=${PROJECT_JUDY_PATH}
  BUILD_IN_SOURCE TRUE
  BUILD_COMMAND ${MAKE}
)
ExternalProject_Get_Property(project_judy install_dir)
add_library(judy STATIC IMPORTED)
set_property(TARGET judy PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libJudy.a)
set_property(TARGET judy PROPERTY POSITION_INDEPENDENT_CODE ON)
include_directories(bft PUBLIC "${install_dir}/include")

add_dependencies(bft judy)
add_dependencies(bft_static judy)
target_link_libraries(bft_bin judy)
target_link_libraries(bft_static judy)
target_link_libraries(bft_shared judy)

# Jemalloc
set(PROJECT_JEMALLOC_PATH ${CMAKE_BINARY_DIR}/jemalloc-5.1.0)
ExternalProject_Add(project_jemalloc
  URL https://github.com/jemalloc/jemalloc/releases/download/5.1.0/jemalloc-5.1.0.tar.bz2
  URL_HASH MD5=1f47a5aff2d323c317dfa4cf23be1ce4
  PREFIX ${PROJECT_JEMALLOC_PATH}
  CONFIGURE_COMMAND ${PROJECT_JEMALLOC_PATH}/src/project_jemalloc/configure --prefix=${PROJECT_JEMALLOC_PATH}
  BUILD_IN_SOURCE TRUE
  BUILD_COMMAND ${MAKE}
)
ExternalProject_Get_Property(project_jemalloc install_dir)
add_library(jemalloc STATIC IMPORTED)
set_property(TARGET jemalloc PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libjemalloc.a)
set_property(TARGET jemalloc PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(bft PUBLIC "${install_dir}/include")

add_dependencies(bft jemalloc)
add_dependencies(bft_static jemalloc)
target_link_libraries(bft_bin jemalloc)
target_link_libraries(bft_static jemalloc)
target_link_libraries(bft_shared jemalloc)

