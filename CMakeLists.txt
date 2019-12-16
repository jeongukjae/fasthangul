cmake_minimum_required(VERSION 3.1)

project(fasthangul VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# files
include_directories(./Includes)
set(SOURCE_CODE
      Sources/jamo.cc)
set(TEST_CODE
      Tests/TestJamo.cc)

# includes
include(./cmake/Testing.cmake)

# fasthangul
add_library(fasthangul SHARED ${SOURCE_CODE})
