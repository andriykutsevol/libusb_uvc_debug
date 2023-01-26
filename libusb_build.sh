#!/bin/bash
set -e
clear


#===================================
# sudo apt update
# sudo apt upgrade
# sudo apt-get install build-essential
# sudo apt-get install autoconf
# sudo apt-get install libtool
# sudo apt install libudev-dev
#===================================

current_dir=`pwd`
working_dir=${current_dir}"/libusb"
build_dir=${working_dir}/build
mkdir -p ${build_dir}
cd ${working_dir}


#===================================


rm -Rf ${build_dir}/*
autoreconf -fiv
cd ${build_dir}


echo "=============================="
echo "configure"
echo "=============================="
../configure --prefix=${build_dir}


echo "=============================="
echo "make"
echo "=============================="
make



echo "=============================="
echo "make install (to local dir): ${build_dir}/lib"
echo "=============================="
make install

echo "=============================="
echo "install (cp, ln -s)"
echo "=============================="


rm -f /usr/lib/libusb-1.0.so.0.3.0
rm -f /usr/lib/libusb-1.0.so.0
rm -f /usr/lib/libusb-1.0.so

cp ${build_dir}/lib/libusb-1.0.so.0.3.0 /lib
ln -s /usr/lib/libusb-1.0.so.0.3.0 /usr/lib/libusb-1.0.so.0
ln -s /usr/lib/libusb-1.0.so.0.3.0 /usr/lib/libusb-1.0.so


