#!/bin/bash
set -e 

if [ "$EUID" -eq 0 ]
  then echo "Do not run as root"
  exit
fi


#===================================
# sudo apt update
# sudo apt upgrade
# sudo apt install libusb-1.0-0-dev
# sudo apt install v4l-utils
# sudo apt install qv4l2
# sudo apt install v4l-conf
# sudo apt install tcpdump
# sudo apt install ffmpeg
# sudo apt install vlc
# sudo apt install mpv
#===================================

current_dir=`pwd`
working_dir=${current_dir}"/libuvc_debug"
build_dir=${working_dir}/build
mkdir -p ${build_dir}
cd ${working_dir}

#===================================

rm -Rf ${build_dir}/*
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


