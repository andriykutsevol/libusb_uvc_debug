#!/bin/bash
set -e

clear

current_dir=`pwd`
working_dir=${current_dir}"/libusb"
build_dir=${working_dir}/build
cd ${build_dir}



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


