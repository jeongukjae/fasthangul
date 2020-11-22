cmake_minimum_required(VERSION 3.1)

project(fasthangul VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# files
include_directories(./include)
set(TEST_CODE
      src/jamo_test.cc)

option(BUILD_TEST "Build the tests" OFF)

if(BUILD_TEST)
  enable_testing()
  add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest")

  include_directories(${gtest_SOURCE_DIR}/include)
  include_directories(${gmock_SOURCE_DIR}/include)

  add_executable(run-test ${TEST_CODE})
  target_link_libraries(run-test gtest gmock gtest_main)

  add_test(NAME run-test COMMAND ./run-test)
endif()
