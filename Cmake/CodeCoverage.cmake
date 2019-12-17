option(BUILD_COVERAGE "Build for the coverage" OFF)

if(BUILD_COVERAGE)
  find_program( GCOV gcov )
  find_program( LCOV_COMMAND lcov )

  if(NOT GCOV)
    message(FATAL_ERROR "gcov not found")
  endif()

  if(NOT LCOV_COMMAND)
    message(FATAL_ERROR "lcov not found")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")

  add_custom_target(coverage
    COMMAND ${LCOV_COMMAND} --directory . --zerocounters
    COMMAND ${LCOV_COMMAND} --capture --initial --directory ../Sources --output-file app_base.info
    COMMAND ./run-test
    COMMAND ${LCOV_COMMAND} --directory . --capture --output-file coverage.info.raw
    COMMAND ${LCOV_COMMAND} --extract coverage.info.raw '**/fasthangul/**' --output-file coverage.info.extracted
    COMMAND ${LCOV_COMMAND} --remove coverage.info.extracted '**/Tests/**' '**/third_party/**' '/usr/*' --output-file lcov.info
    COMMAND ${LCOV_COMMAND} --directory . --zerocounters # cleaning
    COMMAND ${CMAKE_COMMAND} -E remove coverage.info.raw coverage.info.extracted

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()
