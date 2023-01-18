#!/bin/bash
set -e


output_dir=`pwd`"/aver/libuvc/1"
working_dir=`pwd`

#experiment_number vid pid video0 width heigth format_number frame_number fps outpud_dir

./LIBUVC_EXAMPLES_EX.sh 1 1994 4371 video4 1920 1080 1 1 60 ${output_dir}

cd ${working_dir}
./LIBUVC_EXAMPLES_EX.sh 1 1994 4371 video4 1280 720 1 2 60 ${output_dir}

cd ${working_dir}
./LIBUVC_EXAMPLES_EX.sh 1 1994 4371 video4 640 480 1 5 60 ${output_dir}