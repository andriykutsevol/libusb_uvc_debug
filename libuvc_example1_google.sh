#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


ex_num=1
output_dir=`pwd`"/google/libuvc/${ex_num}"
working_dir=`pwd`


# sudo modprobe -r xhci_pci
# sudo modprobe xhci_pci

# v4l2-ctl --all --device /dev/${device_file}
dev_file="video0"


# v4l2-ctl --device /dev/${dev_file} --list-formats-ext


#v4l2-ctl --device /dev/${dev_file} --set-fmt-video=width=1920,height=1080,pixelformat=YUYV
./LIBUVC_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_file} 1920 1080 0 0 30 ${output_dir}

cd ${working_dir}
#v4l2-ctl --device /dev/${dev_file} --set-fmt-video=width=1280,height=720,pixelformat=YUYV
./LIBUVC_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_file} 1280 720 0 1 30 ${output_dir}


cd ${working_dir}
#v4l2-ctl --device /dev/${dev_file} --set-fmt-video=width=640,height=480,pixelformat=YUYV
./LIBUVC_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_file} 640 480 0 0 30 ${output_dir}






#-----------------------
#Make Slice
#-----------------------
working_dir=`pwd`
cd ${working_dir}

dev_name="google"
slices_dir=${working_dir}/slices
#sudo rm -rf ${slices_dir}/*
sudo rm -rf {dev_name}_slice.tar.gz
mkdir -p ${slices_dir}


./make_slice.sh ${working_dir} ${dev_name}
cp ./${dev_name}_slice.tar.gz ${slices_dir}/
rm -rf ./${dev_name}_slice.tar.gz
rm -rf ./${dev_name}_slice