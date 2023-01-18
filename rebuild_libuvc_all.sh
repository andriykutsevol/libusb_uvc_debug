#!/bin/bash
set -e 

root_dir=`pwd`

rm -R ./libuvc_debug
git clone https://github.com/andriykutsevol/libuvc_debug.git --branch hdmi

cd ./libuvc_debug
mkdir ./build
cd ./build
cmake ..
make
