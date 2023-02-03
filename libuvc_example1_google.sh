#!/bin/bash
set -e


ex_num=1
output_dir=`pwd`"/google/libuvc/${ex_num}"
working_dir=`pwd`


# v4l2-ctl --all --device /dev/${device_file}
dev_file="video0"


# v4l2-ctl --device /dev/${dev_file} --list-formats-ext


v4l2-ctl --device /dev/${dev_file} --set-fmt-video=width=1920,height=1080,pixelformat=YUYV
./LIBUVC_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_file} 1920 1080 0 0 30 ${output_dir}

# cd ${working_dir}
# v4l2-ctl --device /dev/${dev_file} --set-fmt-video=width=1280,height=720,pixelformat=YUYV
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_file} 1280 720 0 1 30 ${output_dir}


# cd ${working_dir}
# v4l2-ctl --device /dev/${dev_file} --set-fmt-video=width=640,height=480,pixelformat=YUYV
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_file} 640 480 0 0 30 ${output_dir}