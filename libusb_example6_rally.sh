#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi




# #====================================================
# # 640x480
# #====================================================
# fps=60

# dev_fname=video6

# contol_bInterfaceNumber_mysetting=0;
# bEndpointAddress_mysetting=133;
# bcdUVC_mysetting=256  # 0x100;


# bInterfaceNumber_mysetting=1;
# bmHint_mysetting=1;
# bFormatIndex_mysetting=1;
# bFrameIndex_mysetting=1;
# dwFrameInterval_mysetting=333333;
# wKeyFrameRate_mysetting=0;
# wPFrameRate_mysetting=0;
# wCompQuality_mysetting=0;
# wCompWindowSize_mysetting=0;
# wDelay_mysetting=0;
# dwMaxVideoFrameSize_mysetting=614400;
# dwMaxPayloadTransferSize_mysetting=3060;


# endpoint_bytes_per_packet_mysetting=3060;
# bAlternateSetting_mysetting=6;
# bEndpointAddres_mysetting=129;
# total_transfer_size_mysetting=97920;

# #------------------------------------------------------

# ex_num=6
# output_dir=`pwd`"/rally/libusb/${ex_num}"

# ./LIBUSB_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 640 480 \
# ${contol_bInterfaceNumber_mysetting} \
# ${bEndpointAddress_mysetting} \
# ${bcdUVC_mysetting} \
# ${bInterfaceNumber_mysetting} \
# ${bmHint_mysetting} \
# ${bFormatIndex_mysetting} \
# ${bFrameIndex_mysetting} \
# ${dwFrameInterval_mysetting} \
# ${wKeyFrameRate_mysetting} \
# ${wPFrameRate_mysetting} \
# ${wCompQuality_mysetting} \
# ${wCompWindowSize_mysetting} \
# ${wDelay_mysetting} \
# ${dwMaxVideoFrameSize_mysetting} \
# ${dwMaxPayloadTransferSize_mysetting} \
# ${endpoint_bytes_per_packet_mysetting} \
# ${bAlternateSetting_mysetting} \
# ${bEndpointAddres_mysetting} \
# ${total_transfer_size_mysetting} \
# ${fps} \
# ${output_dir}






# #====================================================
# # 1280x720
# #====================================================
fps=30

dev_fname=video6

contol_bInterfaceNumber_mysetting=0;
bEndpointAddress_mysetting=133;
bcdUVC_mysetting=256  # 0x100;


bInterfaceNumber_mysetting=1;
bmHint_mysetting=1;
bFormatIndex_mysetting=1;
bFrameIndex_mysetting=14;
dwFrameInterval_mysetting=333333;
wKeyFrameRate_mysetting=0;
wPFrameRate_mysetting=0;
wCompQuality_mysetting=0;
wCompWindowSize_mysetting=0;
wDelay_mysetting=0;
dwMaxVideoFrameSize_mysetting=1843200;
dwMaxPayloadTransferSize_mysetting=9216;


endpoint_bytes_per_packet_mysetting=9216;
bAlternateSetting_mysetting=9;
bEndpointAddres_mysetting=129;
total_transfer_size_mysetting=294912;

#------------------------------------------------------

ex_num=6
output_dir=`pwd`"/rally/libusb/${ex_num}"

./LIBUSB_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1280 720 \
${contol_bInterfaceNumber_mysetting} \
${bEndpointAddress_mysetting} \
${bcdUVC_mysetting} \
${bInterfaceNumber_mysetting} \
${bmHint_mysetting} \
${bFormatIndex_mysetting} \
${bFrameIndex_mysetting} \
${dwFrameInterval_mysetting} \
${wKeyFrameRate_mysetting} \
${wPFrameRate_mysetting} \
${wCompQuality_mysetting} \
${wCompWindowSize_mysetting} \
${wDelay_mysetting} \
${dwMaxVideoFrameSize_mysetting} \
${dwMaxPayloadTransferSize_mysetting} \
${endpoint_bytes_per_packet_mysetting} \
${bAlternateSetting_mysetting} \
${bEndpointAddres_mysetting} \
${total_transfer_size_mysetting} \
${fps} \
${output_dir}








# #====================================================
# # 1280x960
# #====================================================
# fps=5

# contol_bInterfaceNumber_mysetting=0;
# bEndpointAddress_mysetting=135;
# bcdUVC_mysetting=256  # 0x100;


# bInterfaceNumber_mysetting=1;
# bmHint_mysetting=1;
# bFormatIndex_mysetting=1;
# bFrameIndex_mysetting=19;
# dwFrameInterval_mysetting=2000000;
# wKeyFrameRate_mysetting=0;
# wPFrameRate_mysetting=0;
# wCompQuality_mysetting=2000;
# wCompWindowSize_mysetting=0;
# wDelay_mysetting=0;
# dwMaxVideoFrameSize_mysetting=2457600;
# dwMaxPayloadTransferSize_mysetting=3060;


# endpoint_bytes_per_packet_mysetting=3060;
# bAlternateSetting_mysetting=11;
# bEndpointAddres_mysetting=129;
# total_transfer_size_mysetting=97920;

# #------------------------------------------------------

# ex_num=6
# output_dir=`pwd`"/rally/libusb/${ex_num}"

# ./LIBUSB_EXAMPLES_EX.sh ${ex_num} 1133 2177 ${dev_fname} 1280 960 \
# ${contol_bInterfaceNumber_mysetting} \
# ${bEndpointAddress_mysetting} \
# ${bcdUVC_mysetting} \
# ${bInterfaceNumber_mysetting} \
# ${bmHint_mysetting} \
# ${bFormatIndex_mysetting} \
# ${bFrameIndex_mysetting} \
# ${dwFrameInterval_mysetting} \
# ${wKeyFrameRate_mysetting} \
# ${wPFrameRate_mysetting} \
# ${wCompQuality_mysetting} \
# ${wCompWindowSize_mysetting} \
# ${wDelay_mysetting} \
# ${dwMaxVideoFrameSize_mysetting} \
# ${dwMaxPayloadTransferSize_mysetting} \
# ${endpoint_bytes_per_packet_mysetting} \
# ${bAlternateSetting_mysetting} \
# ${bEndpointAddres_mysetting} \
# ${total_transfer_size_mysetting} \
# ${fps} \
# ${output_dir}














#------------------------------------------------------

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