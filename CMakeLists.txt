cmake_minimum_required(VERSION 3.1)

project(fasthangul VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/Cmake/Modules/")

# files
include_directories(./Includes)
set(SOURCE_CODE
      Sources/jamo.cc
      Sources/normalizer.cc
      Sources/tokenizer.cc
      Sources/vocab.cc)
set(TEST_CODE
      Tests/TestJamo.cc
      Tests/TestNormalizer.cc
      Tests/TestTokenizer.cc
      Tests/TestVocab.cc)

# includes
include(./Cmake/Testing.cmake)
include(./Cmake/CodeCoverage.cmake)

find_package(MKL REQUIRED)

# fasthangul
include_directories(${MKL_INCLUDE_DIRS})
add_library(fasthangul SHARED ${SOURCE_CODE})
target_link_libraries(fasthangul ${MKL_LIBRARIES})
