if(NOT TBB_ROOT)
	set(TBB_ROOT $ENV{TBBROOT})
endif()

find_path(TBB_INCLUDE_DIR NAMES tbb/tbb.h PATHS ${TBB_ROOT}/include)
find_library(TBB_LIBRARY NAMES tbb PATHS ${TBB_ROOT}/lib)
