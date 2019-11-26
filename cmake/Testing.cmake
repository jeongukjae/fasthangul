option(BUILD_TEST "Build the tests" ON)

if(BUILD_TEST)
  enable_testing()
  add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest")

  include_directories(${PROJECT_SOURCE_DIR}/fasthangul)
  include_directories(${gtest_SOURCE_DIR}/include)
  include_directories(${gmock_SOURCE_DIR}/include)

  add_executable(run-test ${TEST_CODE})
  target_link_libraries(run-test fasthangul)
  target_link_libraries(run-test gtest gmock gtest_main)
endif()
