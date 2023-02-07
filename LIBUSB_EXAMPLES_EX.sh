#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi



#---------------------------------------------------------
# NOTES:


#---------------------------------------------------------

example_number=${1}
device_vidpid=${2}":"${3}
device_vid=${2}
device_pid=${3}
device_file=${4}
videon=${device_file}
resolution=${5}":"${6}
width=${5}
height=${6}


echo "example number:       " ${example_number}
echo "device_vidpid:        " ${device_vidpid}
echo "device file:          " ${device_file}
echo "resolution:           " ${resolution}


contol_bInterfaceNumber_mysetting=${7};
bEndpointAddress_mysetting=${8};
bcdUVC_mysetting=${9}  # 0x100;


bInterfaceNumber_mysetting=${10};
bmHint_mysetting=${11};
bFormatIndex_mysetting=${12};
bFrameIndex_mysetting=${13};
dwFrameInterval_mysetting=${14};
wKeyFrameRate_mysetting=${15};
wPFrameRate_mysetting=${16};
wCompQuality_mysetting=${17};
wCompWindowSize_mysetting=${18};
wDelay_mysetting=${19};
dwMaxVideoFrameSize_mysetting=${20};
dwMaxPayloadTransferSize_mysetting=${21};


endpoint_bytes_per_packet_mysetting=${22};
bAlternateSetting_mysetting=${23};
bEndpointAddres_mysetting=${24};
total_transfer_size_mysetting=${25};

output_dir=${26}"/"${width}"x"${height}


host_results=`pwd`/host/host_results
binary_path=`pwd`"/libuvc_debug/build"
root_dir=`pwd`


#output_dir:            /home/dgnet/experiment/3/c270/libuvc/1/640x480
#binary_path:           /home/dgnet/experiment/3/libuvc_debug/build

mkdir -p ${output_dir}
mkdir -p ${host_results}
rm -fR ${output_dir}/*
rm -fR ${binary_path}/out


#======================================================

./example 1133 2085 \
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
${total_transfer_size_mysetting}



#======================================================
