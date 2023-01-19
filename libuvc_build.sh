#!/bin/bash
set -e 

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

echo "=============================="
echo "make"
echo "=============================="
make ..


# root_dir=`pwd`

# rm -R ./libuvc_debug
# git clone https://github.com/andriykutsevol/libuvc_debug.git --branch hdmi

# cd ./libuvc_debug
# mkdir ./build
# cd ./build
# cmake ..
# make
