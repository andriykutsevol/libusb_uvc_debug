#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


    # sudo modprobe -r xhci_pci
    # sudo modprobe xhci_pci
    # v4l2-ctl --all --device /dev/video0


dev_folder="google"


#====================================================
# 640x480
#====================================================
fps=60
width=640
height=480

dev_fname=video0

bInterfaceNumber_contol_mysetting=0;
bEndpointAddress_interrupt_mysetting=131;
bcdUVC_mysetting=256  # 0x100;


bInterfaceNumber_streaming_mysetting=1;
bmHint_mysetting=1;
bFormatIndex_mysetting=1;
bFrameIndex_mysetting=1;
dwFrameInterval_mysetting=166666;
wKeyFrameRate_mysetting=0;
wPFrameRate_mysetting=0;
wCompQuality_mysetting=0;
wCompWindowSize_mysetting=0;
wDelay_mysetting=32;
dwMaxVideoFrameSize_mysetting=4147200;
dwMaxPayloadTransferSize_mysetting=49152;


endpoint_bytes_per_packet_mysetting=49152;
bAlternateSetting_mysetting=6;
bEndpointAddress_iso_mysetting=129;
total_transfer_size_mysetting=1572864;

#------------------------------------------------------

ex_num=6
output_dir=`pwd`"/${dev_folder}/libusb/${ex_num}"

./LIBUSB_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_fname} ${width} ${height} \
${bInterfaceNumber_contol_mysetting} \
${bEndpointAddress_interrupt_mysetting} \
${bcdUVC_mysetting} \
${bInterfaceNumber_streaming_mysetting} \
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
${bEndpointAddress_iso_mysetting} \
${total_transfer_size_mysetting} \
${fps} \
${output_dir}





# #====================================================
# # 1280x720
# #====================================================
# fps=15

# dev_fname=video6

# bInterfaceNumber_contol_mysetting=0;
# bEndpointAddress_interrupt_mysetting=133;
# bcdUVC_mysetting=256  # 0x100;


# bInterfaceNumber_streaming_mysetting=1;
# bmHint_mysetting=1;
# bFormatIndex_mysetting=1;
# bFrameIndex_mysetting=14;
# dwFrameInterval_mysetting=333333;
# wKeyFrameRate_mysetting=0;
# wPFrameRate_mysetting=0;
# wCompQuality_mysetting=0;
# wCompWindowSize_mysetting=0;
# wDelay_mysetting=24576;
# dwMaxVideoFrameSize_mysetting=1843200;
# dwMaxPayloadTransferSize_mysetting=9216;


# endpoint_bytes_per_packet_mysetting=9216;
# bAlternateSetting_mysetting=8;
# bEndpointAddress_iso_mysetting=129;
# total_transfer_size_mysetting=294912;

# #------------------------------------------------------

# ex_num=6
# output_dir=`pwd`"/rally/libusb/${ex_num}"

# ./LIBUSB_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_fname} 1280 720 \
# ${bInterfaceNumber_contol_mysetting} \
# ${bEndpointAddress_interrupt_mysetting} \
# ${bcdUVC_mysetting} \
# ${bInterfaceNumber_streaming_mysetting} \
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
# ${bEndpointAddress_iso_mysetting} \
# ${total_transfer_size_mysetting} \
# ${fps} \
# ${output_dir}


# # $: ffmpeg -f rawvideo -pix_fmt yuyv422 -s:v 1280x720 -r 30 -i ./out.yuv ./output.avi
# # [rawvideo @ 0x5632fa02b180] Packet corrupt (stream = 0, dts = 298).
# # ./out.yuv: corrupt input packet in stream 0
# # [rawvideo @ 0x5632fa037a40] Invalid buffer size, packet size 481008 < expected frame_size 1843200
# # Error while decoding stream #0:0: Invalid argument
# # frame=  298 fps=0.0 q=31.0 Lsize=     805kB time=00:00:09.93 bitrate= 664.3kbits/s speed=39.9x    
# # video:793kB audio:0kB subtitle:0kB other streams:0kB global headers:0kB muxing overhead: 1.599913%





# #====================================================
# # 1920x1080
# #====================================================
# fps=15

# dev_fname=video6

# bInterfaceNumber_contol_mysetting=0;
# bEndpointAddress_interrupt_mysetting=133;
# bcdUVC_mysetting=256  # 0x100;


# bInterfaceNumber_streaming_mysetting=1;
# bmHint_mysetting=1;
# bFormatIndex_mysetting=1;
# bFrameIndex_mysetting=16;
# dwFrameInterval_mysetting=333333;
# wKeyFrameRate_mysetting=0;
# wPFrameRate_mysetting=0;
# wCompQuality_mysetting=0;
# wCompWindowSize_mysetting=0;
# wDelay_mysetting=24576;
# dwMaxVideoFrameSize_mysetting=4147200;
# dwMaxPayloadTransferSize_mysetting=18432;


# endpoint_bytes_per_packet_mysetting=18432;
# bAlternateSetting_mysetting=11;
# bEndpointAddress_iso_mysetting=129;
# total_transfer_size_mysetting=589824;

# #------------------------------------------------------

# ex_num=6
# output_dir=`pwd`"/rally/libusb/${ex_num}"

# ./LIBUSB_EXAMPLES_EX.sh ${ex_num} 6353 20510 ${dev_fname} 1920 1080 \
# ${bInterfaceNumber_contol_mysetting} \
# ${bEndpointAddress_interrupt_mysetting} \
# ${bcdUVC_mysetting} \
# ${bInterfaceNumber_streaming_mysetting} \
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
# ${bEndpointAddress_iso_mysetting} \
# ${total_transfer_size_mysetting} \
# ${fps} \
# ${output_dir}














#------------------------------------------------------

#-----------------------
#Make Slice
#-----------------------
working_dir=`pwd`
cd ${working_dir}


slices_dir=${working_dir}/slices
#sudo rm -rf ${slices_dir}/*
sudo rm -rf {dev_folder}_slice.tar.gz
mkdir -p ${slices_dir}


./make_slice.sh ${working_dir} ${dev_folder}
cp ./${dev_folder}_slice.tar.gz ${slices_dir}/
rm -rf ./${dev_folder}_slice.tar.gz
rm -rf ./${dev_folder}_slice