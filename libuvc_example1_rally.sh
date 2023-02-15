#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


# ./libuvc_example1_rally.sh


# ffplay -f mjpeg -i ./out.mjpeg

ex_num=1

output_dir=`pwd`"/rally/libuvc/${ex_num}"

dev_fname=video6


# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 640 480 0 0 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 160 120 0 1 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 176 144 0 2 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 320 180 0 3 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 320 240 0 4 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 352 288 0 5 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 480 270 0 6 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 640 360 0 7 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 800 448 0 8 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 800 600 0 9 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 848 480 0 10 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 960 540 0 11 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1024 576 0 12 30 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1280 720 0 13 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1600 896 0 14 30 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1920 1080 0 15 30 ${output_dir}












 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 640 480 1 0 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 160 120 1 1 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 176 144 1 2 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 320 180 1 3 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 320 240 1 4 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 352 288 1 5 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 480 270 1 6 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 640 360 1 7 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 800 448 1 8 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 800 600 1 9 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 848 480 1 10 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 960 540 1 11 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1024 576 1 12 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1280 720 1 13 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1600 896 1 14 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1920 1080 1 15 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 2560 1440 1 16 30 ${output_dir}

 ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 3840 2160 1 17 30 ${output_dir}

# ffprobe ./out
# [mjpeg @ 0x564bed405900] Format mjpeg detected only with low score of 12, misdetection possible!
# Input #0, mjpeg, from './out':
#   Duration: N/A, bitrate: N/A
#   Stream #0:0: Video: mjpeg (Baseline), yuvj422p(pc, bt470bg/unknown/unknown), 3840x2160, 25 tbr, 1200k tbn, 25 tbc









#-----------------------
#Make Slice
#-----------------------
working_dir=`pwd`
cd ${working_dir}

dev_name="rally"
slices_dir=${working_dir}/slices
#sudo rm -rf ${slices_dir}/*
sudo rm -rf {dev_name}_slice.tar.gz
mkdir -p ${slices_dir}


./make_slice.sh ${working_dir} ${dev_name}
cp ./${dev_name}_slice.tar.gz ${slices_dir}/
rm -rf ./${dev_name}_slice.tar.gz
rm -rf ./${dev_name}_slice