#!/bin/bash

set -e

git clone https://github.com/wjakob/tbb.git
cd tbb/build

cmake ..
make -j
sudo make install

cd ../..
rm -rf tbb
