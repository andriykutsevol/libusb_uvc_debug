#!/bin/bash
set -e
# ./libuvc_example1_rally.sh

ex_num=1

output_dir=`pwd`"/rally/libuvc/${ex_num}"

#./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 1920 1080 0 15 30 ${output_dir}

# ffplay -f mjpeg -i ./out.mjpeg
./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 3840 2160 1 17 30 ${output_dir}

# ffprobe ./out
# [mjpeg @ 0x564bed405900] Format mjpeg detected only with low score of 12, misdetection possible!
# Input #0, mjpeg, from './out':
#   Duration: N/A, bitrate: N/A
#   Stream #0:0: Video: mjpeg (Baseline), yuvj422p(pc, bt470bg/unknown/unknown), 3840x2160, 25 tbr, 1200k tbn, 25 tbc
