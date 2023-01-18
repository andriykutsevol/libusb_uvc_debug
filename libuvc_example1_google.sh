#!/bin/bash
set -e


output_dir=`pwd`"/google/libuvc/1"
working_dir=`pwd`

# v4l2-ctl --device /dev/video2 --list-formats-ext


v4l2-ctl --device /dev/video2 --set-fmt-video=width=1920,height=1080,pixelformat=YUYV
./LIBUVC_EXAMPLES_EX.sh 1 6353 20510 video2 1920 1080 0 0 60 ${output_dir}

cd ${working_dir}
v4l2-ctl --device /dev/video2 --set-fmt-video=width=1280,height=720,pixelformat=YUYV
./LIBUVC_EXAMPLES_EX.sh 1 6353 20510 video2 1280 720 0 1 60 ${output_dir}


cd ${working_dir}
v4l2-ctl --device /dev/video2 --set-fmt-video=width=640,height=480,pixelformat=YUYV
./LIBUVC_EXAMPLES_EX.sh 1 6353 20510 video2 640 480 0 0 60 ${output_dir}