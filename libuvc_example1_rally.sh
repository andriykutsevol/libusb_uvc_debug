#!/bin/bash
set -e
# ./libuvc_example1_rally.sh

ex_num=1

output_dir=`pwd`"/rally/libuvc/${ex_num}"

# ffplay -f mjpeg -i ./out.mjpeg


# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 640 480 1 0 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 160 120 1 1 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 176 144 1 2 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 320 180 1 3 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 320 240 1 4 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 352 288 1 5 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 480 270 1 6 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 640 360 1 7 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 800 448 1 8 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 800 600 1 9 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 848 480 1 10 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 960 540 1 11 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 1024 576 1 12 30 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 1280 720 1 13 30 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 1600 896 1 14 30 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 1920 1080 1 15 30 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 2560 1440 1 16 30 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 3840 2160 1 17 30 ${output_dir}

# ffprobe ./out
# [mjpeg @ 0x564bed405900] Format mjpeg detected only with low score of 12, misdetection possible!
# Input #0, mjpeg, from './out':
#   Duration: N/A, bitrate: N/A
#   Stream #0:0: Video: mjpeg (Baseline), yuvj422p(pc, bt470bg/unknown/unknown), 3840x2160, 25 tbr, 1200k tbn, 25 tbc
