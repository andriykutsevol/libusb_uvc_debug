#!/bin/bash
set -e

# ./make_slice.sh /home/dgnet/qemu/guest/libusb_uvc_debug/rally/libuvc 1

if [ -z "$1" ]
  then
    echo "Two arguments required"
    exit
fi

if [ -z "$2" ]
  then
    echo "Two arguments required"
    exit
fi


working_dir=${1}
target_dir=${2}

rm -rf ${working_dir}/${target_dir}_slice
rm -rf ${working_dir}/${target_dir}_slice.zip

cd ${working_dir}
cp -r ./${target_dir} ./${target_dir}_slice

cd ./${target_dir}_slice

find . -type f -name 'out' -exec rm {} +
find . -type f -name 'output.avi' -exec rm {} +

cd ../

tar -czf ./${target_dir}_slice.tar.gz ./${target_dir}_slice





