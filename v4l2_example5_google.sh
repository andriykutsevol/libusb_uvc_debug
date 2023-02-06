#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


output_dir=`pwd`"/google/v4l2-examples/5"
working_dir=`pwd`

./V4L2_EXAMPLES_EX.sh 5 6353 20510 video2 1920 1080 0 -1 60 ${output_dir}

cd ${working_dir}
./V4L2_EXAMPLES_EX.sh 5 6353 20510 video2 1280 720 0 -1 60 ${output_dir}

cd ${working_dir}
./V4L2_EXAMPLES_EX.sh 5 6353 20510 video2 640 480 0 -1 60 ${output_dir}