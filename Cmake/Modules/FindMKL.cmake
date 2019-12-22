# Check https://software.intel.com/en-us/mkl-macos-developer-guide-linking-in-detail
#
# To use the Intel® MKL FFT, Trigonometric Transform, or Poisson, Laplace, and HelmholtzSolver routines,
# link also the math support system library by adding "-lm" to the link line.
#
# The libiomp5 library relies on the native pthread library for multi-threading. Any time libiomp5 is
# required, add -lpthread to your link line afterwards (the order of listing libraries is important).
#
# The libtbb library relies on the compiler libstdc++ library for C++ support. Any time libtbb is required,
# add -lstdc++ to your link line afterwards (the order of listing libraries is important).

option(USE_TBB "USE TBB with MKL" ON)
set(MKL_ROOT "/opt/intel/mkl")

set(mkl_lib "libmkl_intel_lp64.dylib")
set(mkl_core_lib "libmkl_core.dylib")

if (USE_TBB)
  set(threading_lib "libmkl_tbb_thread.dylib")
  set(threading_extra "stdc++")
else()
  set(threading_lib "libmkl_intel_thread.dylib")
  set(threading_extra "pthread")
endif()

find_path(MKL_INCLUDE_DIR NAMES mkl.h HINTS ${MKL_ROOT}/include)
find_library(MKL_INTERFACE_LIBRARY
             NAMES ${mkl_lib}
             PATHS ${MKL_ROOT}/lib
                   ${MKL_ROOT}/lib/intel64
             NO_DEFAULT_PATH)
find_library(MKL_CORE_LIBRARY
             NAMES ${mkl_core_lib}
             PATHS ${MKL_ROOT}/lib
                   ${MKL_ROOT}/lib/intel64
             NO_DEFAULT_PATH)
find_library(MKL_THREADING_LIBRARY
             NAMES ${threading_lib}
             PATHS ${MKL_ROOT}/lib
                   ${MKL_ROOT}/lib/intel64
             NO_DEFAULT_PATH)

set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
set(MKL_LIBRARIES
        ${MKL_INTERFACE_LIBRARY}
        ${MKL_CORE_LIBRARY}
        ${threading_extra}
        ${MKL_THREADING_LIBRARY})

message(STATUS "MKL INCLUDE DIRS: ${MKL_INCLUDE_DIRS}")
message(STATUS "MKL LIBS: ${MKL_LIBRARIES}")
