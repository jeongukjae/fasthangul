cmake_minimum_required(VERSION 3.1)

project(fasthangul VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake/modules/")

# files
include_directories(./Includes)
set(SOURCE_CODE
      src/jamo.cc)
set(TEST_CODE
      src/jamo_test.cc)

# includes
include(./cmake/Testing.cmake)
include(./cmake/CodeCoverage.cmake)

# link libraries
find_package(MKL REQUIRED)
find_package(TBB REQUIRED)

include_directories(${MKL_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIR})

# fasthangul
add_library(fasthangul SHARED ${SOURCE_CODE})
target_link_libraries(fasthangul ${MKL_LIBRARIES} ${TBB_LIBRARY})
