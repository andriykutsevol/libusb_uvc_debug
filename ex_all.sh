#!/bin/bash
set -e


working_dir=`pwd`
device_dir=${working_dir}/google
slices_dir=${working_dir}/slices

#===================================================================
# cleanup
#===================================================================

sudo rm -rf ${device_dir}/*
sudo rm -rf ${slices_dir}/*

#===================================================================
# libuvc_example1_google.sh
#===================================================================

ex_num=1
output_dir=${device_dir}/libuvc/${ex_num}



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



#===================================================================
# libuvc_example7_google.sh
#===================================================================




#===================================================================
# Make Slice
#===================================================================
cd ${working_dir}

#-----------------------
#google
#-----------------------

./make_slice.sh ${working_dir} "google"

cp ./google_slice.tar.gz ./slices_dir/

rm -rf ./google_slice.tar.gz
rm -rf ./google_slice

#-----------------------
#rally
#-----------------------