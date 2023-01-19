#!/bin/bash
set -e 

#===================================
# sudo apt update
# sudo apt upgrade
# sudo apt install libusb-1.0-0-dev
#===================================


current_dir=`pwd`
working_dir=${current_dir}"/libuvc_debug"
build_dir=${working_dir}/build
cd ${working_dir}

#===================================

rm -Rf ${build_dir}
mkdir ${build_dir}
cd ${build_dir}
#===================================

echo "=============================="
echo "cmake"
echo "=============================="
cmake ..
sync
sync

echo "=============================="
echo "make"
echo "=============================="
make


