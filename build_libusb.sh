#!/bin/bash
set -e

clear

# git clone https://github.com/andriykutsevol/libusb.git --branch=mine_examples
# autoreconf -fiv
# 


rm -f /home/dgnet/libusb/out.txt
rm -f /home/dgnet/libusb/uvc_out.txt

build_dir="/home/dgnet/experiment/7/libusb/build"

cd ${build_dir}

#git pull


echo "=============================="
echo "make"
echo "=============================="


make

echo "=============================="
echo "make install (to local dir)"
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


