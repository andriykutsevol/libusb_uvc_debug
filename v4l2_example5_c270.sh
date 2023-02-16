#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


ex_num=5
dev_name="c270"
dev_file="video0"

output_dir=`pwd`"/${dev_file}/v4l2-examples/${ex_num}"


#-----------------------------------------------------------------------------------

working_dir=`pwd`
slices_dir=${working_dir}/slices
mkdir -p ${slices_dir}


#-----------------------------------------------------------------------------------




cd ${working_dir}
./V4L2_EXAMPLES_EX.sh ${ex_num} 1133 2085 ${dev_file} 1280 720 0 -1 10 ${output_dir}

cd ${working_dir}
./V4L2_EXAMPLES_EX.sh ${ex_num} 1133 2085 ${dev_file} 1280 960 0 -1 10 ${output_dir}


# cd ${working_dir}
# ./V4L2_EXAMPLES_EX.sh ${ex_num} 1133 2085 video2 640 480 0 -1 60 ${output_dir}




#-----------------------
#Make Slice
#-----------------------
cd ${working_dir}


./make_slice.sh ${working_dir} ${dev_name}
cp ./${dev_name}_slice.tar.gz ${slices_dir}/
rm -rf ./${dev_name}_slice.tar.gz
rm -rf ./${dev_name}_slice

