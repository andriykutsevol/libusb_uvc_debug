#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


ex_num=2

output_dir=`pwd`"/c270/libuvc/${ex_num}"

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 480 0 0 15 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1280 720 0 17 15 ${output_dir}