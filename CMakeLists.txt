cmake_minimum_required(VERSION 3.1)

project(fasthangul VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# files
set(SOURCE_CODE
      fasthangul/PythonExtension.hh
      fasthangul/jamo.hh
      fasthangul/jamo.cc)
set(TEST_CODE
      tests/test_jamo.cc)

# includes
include(./cmake/Python.cmake)
include(./cmake/Testing.cmake)

# fasthangul
add_library(fasthangul SHARED ${SOURCE_CODE})
target_link_libraries(fasthangul ${PYTHON_LIBRARIES})
