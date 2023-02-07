#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


ex_num=1

output_dir=`pwd`"/c270/libuvc/${ex_num}"


#-----------------------------------------------------------------------------------

working_dir=`pwd`
slices_dir=${working_dir}/slices
mkdir -p ${slices_dir}


#-----------------------------------------------------------------------------------
# YUYV

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 480 0 0 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 160 120 0 1 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 176 144 0 2 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 320 176 0 3 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 320 240 0 4 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 352 288 0 5 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 432 240 0 6 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 544 288 0 7 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 360 0 8 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 752 416 0 9 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 800 448 0 10 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 800 600 0 11 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 864 480 0 12 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 960 544 0 13 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 960 720 0 14 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1024 576 0 15 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1184 656 0 16 15 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1280 720 0 17 15 ${output_dir}

./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1280 960 0 18 15 ${output_dir}

#-----------------------------------------------------------------------------------
#MJPEG


# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 480 1 0 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 160 120 1 1 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 176 144 1 2 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 320 176 1 3 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 320 240 1 4 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 352 288 1 5 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 432 240 1 6 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 544 288 1 7 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 360 1 8 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 752 416 1 9 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 800 448 1 10 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 800 600 1 11 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 864 480 1 12 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 960 544 1 13 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 960 720 1 14 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1024 576 1 15 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1184 656 1 16 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1280 720 1 17 15 ${output_dir}

# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1280 960 1 18 15 ${output_dir}



#-----------------------
#Make Slice
#-----------------------
cd ${working_dir}
dev_name="c270"

./make_slice.sh ${working_dir} ${dev_name}
cp ./${dev_name}_slice.tar.gz ${slices_dir}/
rm -rf ./${dev_name}_slice.tar.gz
rm -rf ./${dev_name}_slice
