#!/bin/bash
if [ "$(uname)" == "Darwin" ]; then
  echo "Install MKL On MacOS"
  export MKL_LIB_FILE=mkl-2019.5-intel_281
  export MKL_INC_FILE=mkl-include-2019.5-281
  curl -SLO https://anaconda.org/intel/mkl/2019.5/download/osx-64/${MKL_LIB_FILE}.tar.bz2
  curl -SLO https://anaconda.org/anaconda/mkl-include/2019.5/download/osx-64/${MKL_INC_FILE}.tar.bz2

  mkdir ${MKL_LIB_FILE} ${MKL_INC_FILE}
  tar -xvjf ${MKL_LIB_FILE}.tar.bz2 -C ${MKL_LIB_FILE}
  tar -xvjf ${MKL_INC_FILE}.tar.bz2 -C ${MKL_INC_FILE}

  mv ${MKL_LIB_FILE}/lib /opt/intel/mkl/lib
  mv ${MKL_INC_FILE}/include /opt/intel/mkl/include
else
fi
