#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


output_dir=`pwd`"/aver/v4l2-examples/5"
working_dir=`pwd`

#experiment_number vid pid video0 width heigth format_number frame_number fps outpud_dir

./V4L2_EXAMPLES_EX.sh 5 1994 4371 video4 1920 1080 1 -1 60 ${output_dir}

cd ${working_dir}
./V4L2_EXAMPLES_EX.sh 5 1994 4371 video4 1280 720 1 -1 60 ${output_dir}

cd ${working_dir}
./V4L2_EXAMPLES_EX.sh 5 1994 4371 video4 640 480 1 1- 60 ${output_dir}

