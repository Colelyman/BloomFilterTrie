cmake_minimum_required(VERSION 3.1)
include(ExternalProject)

project(bloomfiltertrie)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(bft "")

set(PROJECT_JUDY_PATH ${CMAKE_BINARY_DIR}/judy-1.0.5)
ExternalProject_Add(project_judy
  URL https://downloads.sourceforge.net/project/judy/judy/Judy-1.0.5/Judy-1.0.5.tar.gz
  PREFIX ${PROJECT_JUDY_PATH}
  # PREFIX=${PROJECT_JUDY_PATH}
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
target_link_libraries(bft judy)

set(PROJECT_JEMALLOC_PATH ${CMAKE_BINARY_DIR}/jemalloc-5.1.0)
ExternalProject_Add(project_jemalloc
  URL https://github.com/jemalloc/jemalloc/releases/download/5.1.0/jemalloc-5.1.0.tar.bz2
  PREFIX ${PROJECT_JEMALLOC_PATH}
  # PREFIX=${PROJECT_JEMALLOC_PATH}
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
target_link_libraries(bft jemalloc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c99 -msse4.1 -mpopcnt -DXXH_NAMESPACE=BFT_HASH_")

target_include_directories(bft PUBLIC "${CMAKE_SOURCE_DIR}/include")
include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/include/CMakeLists.txt)
