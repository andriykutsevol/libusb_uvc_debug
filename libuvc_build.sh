#!/bin/bash
set -e 
# ./libuvc_build.sh

#===================================
# sudo apt update
# sudo apt upgrade
# sudo apt install libusb-1.0-0-dev
# sudo apt install v4l-utils
# sudo apt install tcpdump
# sudo apt install ffmpeg
# sudo apt install vlc
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


