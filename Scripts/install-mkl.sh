#!/bin/bash

set -e

if [ "$(uname)" == "Darwin" ]; then
  echo "Install MKL On MacOS"
  export PLATFORM=osx-64
else
  echo "Install MKL On Linux"
  export PLATFORM=linux-64
fi

export MKL_LIB_FILE=mkl-2019.5-intel_281
export MKL_INC_FILE=mkl-include-2019.5-281
curl -SLO https://anaconda.org/intel/mkl/2019.5/download/${PLATFORM}/${MKL_LIB_FILE}.tar.bz2
curl -SLO https://anaconda.org/anaconda/mkl-include/2019.5/download/${PLATFORM}/${MKL_INC_FILE}.tar.bz2

mkdir ${MKL_LIB_FILE} ${MKL_INC_FILE}
tar -xvjf ${MKL_LIB_FILE}.tar.bz2 -C ${MKL_LIB_FILE}
tar -xvjf ${MKL_INC_FILE}.tar.bz2 -C ${MKL_INC_FILE}

sudo mkdir -p /opt/intel/mkl
sudo chown $(id -u):$(id -g) /opt/intel/mkl
mv ${MKL_LIB_FILE}/lib /opt/intel/mkl/lib
mv ${MKL_INC_FILE}/include /opt/intel/mkl/include
