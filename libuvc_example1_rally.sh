#!/bin/bash
set -e


ex_num=1

output_dir=`pwd`"/rally/libuvc/${ex_num}"

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 1920 1080 0 15 30 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2177 video0 3840 2160 1 17 30 ${output_dir}