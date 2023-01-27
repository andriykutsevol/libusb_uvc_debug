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

cd ${working_dir}
cp -r ./${target_dir} ./${target_dir}_slice

cd ./${target_dir}_slice

find . -type f -name 'out' -exec rm {} +





