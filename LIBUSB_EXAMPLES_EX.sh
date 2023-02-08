#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi



#---------------------------------------------------------
# NOTES:


#---------------------------------------------------------

istcpdump=1

#---------------------------------------------------------


sudo modprobe -r uvcvideo
sleep 2
sudo modprobe uvcvideo
sleep 2


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
fps=${26}

output_dir=${27}"/"${width}"x"${height}
host_results=`pwd`/host/host_results
binary_path=`pwd`"/libusb_examples/${example_number}/build"
root_dir=`pwd`



#---------------------------------------------------------

clear
echo "new new new new new new new new"
echo "new new new new new new new new"

#---------------------------------------------------------




#output_dir:            /home/dgnet/experiment/3/c270/libuvc/1/640x480
#binary_path:           /home/dgnet/experiment/3/libuvc_debug/build

mkdir -p ${output_dir}
mkdir -p ${host_results}
rm -fR ${output_dir}/*
rm -fR ${binary_path}/out
rm -fR ${binary_path}/out.yuv
rm -fR ${binary_path}/out.mjpg
rm -fR ${binary_path}/output.avi



#======================================================

echo -n "${device_file}: " > ${output_dir}/ex_out.txt 2>&1

v4l2-ctl --all --device /dev/${device_file} | grep "Card type" >> ${output_dir}/ex_out.txt 2>&1

echo "=============================" >> ${output_dir}/ex_out.txt 2>&1



v4l2-ctl --all --device /dev/${device_file} >> ${output_dir}/ex_out.txt 2>&1


echo "=============================" >> ${output_dir}/ex_out.txt 2>&1
echo "=============================" >> ${output_dir}/ex_out.txt 2>&1

#======================================================

dmesg -c > /dev/null 2>&1
clear
sleep 1


#======================================================

cd ${binary_path}
make

echo "================================="
echo "================================="
echo "================================="

sleep 5

#======================================================


cd ${root_dir}


#-------------------------------
# tcpdump
if [ ${istcpdump} -ne "0" ]; then
    modprobe usbmon
    tcpdump -i usbmon0 -w ${output_dir}/tcpdump1.pcap &
    tcpdump1_pid=$!
fi 
#-------------------------------



echo "modprobe -r uvcvideo" >  ${output_dir}/dmesg.txt
modprobe -r uvcvideo
echo "" >>  ${output_dir}/dmesg.txt
dmesg >> ${output_dir}/dmesg.txt
dmesg -c > /dev/null 2>&1
clear


echo "" >>  ${output_dir}/dmesg.txt
echo "" >>  ${output_dir}/dmesg.txt

#-------------------------------
# tcpdump
if [ ${istcpdump} -ne "0" ]; then
    echo "kill tcpdump1: $tcpdump1_pid"
    kill -9 $tcpdump1_pid

fi 
#-------------------------------

dmesg -c > /dev/null 2>&1
clear
sleep 3

#======================================================


#-------------------------------
# tcpdump
if [ ${istcpdump} -ne "0" ]; then
    modprobe usbmon
    tcpdump -i usbmon0 -w ${output_dir}/tcpdump2.pcap &
    tcpdump2_pid=$!
fi 
#-------------------------------


echo "sudo modprobe uvcvideo" >>  ${output_dir}/dmesg.txt
echo "" >>  ${output_dir}/dmesg.txt
sudo modprobe uvcvideo
dmesg >> ${output_dir}/dmesg.txt
sleep 3

#-------------------------------
# tcpdump
if [ ${istcpdump} -ne "0" ]; then
    echo "kill tcpdump2: $tcpdump2_pid"
    kill -9 $tcpdump2_pid
fi 
#-------------------------------

dmesg -c > /dev/null 2>&1
clear
sleep 3

#======================================================


echo "" >>  ${output_dir}/dmesg.txt
echo "" >>  ${output_dir}/dmesg.txt
echo "libuvc_"${example_number} >> ${output_dir}/dmesg.txt
echo "" >>  ${output_dir}/dmesg.txt


#-------------------------------
# tcpdump
if [ ${istcpdump} -ne "0" ]; then
    modprobe usbmon
    tcpdump -i usbmon0 -w ${output_dir}/tcpdump3.pcap &
    tcpdump3_pid=$!
fi 
#-------------------------------




#======================================================
#======================================================
sleep 2

export LD_LIBRARY_PATH=${binary_path}:$LD_LIBRARY_PATH

echo "========================" >> ${output_dir}/ex_out.txt 2>&1
echo "========================" >> ${output_dir}/ex_out.txt 2>&1
echo "./example_${example_number} ${device_vid} ${device_pid}" >> ${output_dir}/ex_out.txt 2>&1
echo "========================" >> ${output_dir}/ex_out.txt 2>&1
echo "========================" >> ${output_dir}/ex_out.txt 2>&1

cd ${binary_path}
rm -fR ./out
rm -fR ./out.yuv
rm -fR ./out.mjpg
rm -fR ./output.avi

./example ${device_vid} ${device_pid} \
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
>> ${output_dir}/ex_out.txt 2>&1


cd ${output_dir}
sync
sync

sleep 1

# TODO: BUG in the example.c code. It does not release the device properly.


echo "==========================="
echo "ffmpeg -f rawvideo -pix_fmt yuyv422 -s:v ${width}x${height} -r ${fps} -i ${binary_path}/out.yuv ${output_dir}/output.avi"
echo "==========================="

ffmpeg -f rawvideo -pix_fmt yuyv422 -s:v ${width}x${height} -r ${fps} -i ${binary_path}/out.yuv ${output_dir}/output.avi
cp ${binary_path}/out.yuv ${output_dir}

#======================================================
#======================================================

#-------------------------------
# tcpdump
if [ ${istcpdump} -ne "0" ]; then
    echo "kill tcpdump3: $tcpdump3_pid"
    kill -9 $tcpdump3_pid
fi 
#-------------------------------

dmesg >> ${output_dir}/dmesg.txt
dmesg -c > /dev/null 2>&1
clear
sync
sync
sleep 1


#======================================================


mkdir -p ${output_dir}/host_results

sudo chmod -R a+rwx ${host_results}
sudo chmod -R a+rwx ${output_dir}


#======================================================
    # If we run this on host - it will be empty files.
# sudo cat /dev/null > ${host_results}/D_output.txt
# sudo cat /dev/null > ${host_results}/dmesg_host.txt
sudo echo "" > ${host_results}/D_output.txt
sudo echo "" > ${host_results}/dmesg_host.txt

if [ "$(ls -A ${output_dir}/host_results)" ]; then
    echo "Copy host_resluts to output_dir"
    cp ${host_results}/* ${output_dir}/host_results/
fi


#======================================================

sync
sync
sync
sleep 1


# #======================================================

# tar --exclude='*.pcap' -czf ./${videon}.tar.gz ./*

# #======================================================

sync
sync
exit 0