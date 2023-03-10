#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


#---------------------------------------------------------
# NOTES:
# 1)
    # Sometime it cannot change format from the first attempt, and you need to run test twice to make it works.

# 2)
    # sudo modprobe -r xhci_pci
    # sudo modprobe xhci_pci
    # v4l2-ctl --all --device /dev/video0

# 3) ffmpeg -f rawvideo -pix_fmt yuyv422 -s:v 1280x720 -r 30 -i ./out.yuv ./output.avi


#---------------------------------------------------------

# Video4Linux (V4L for short) is a collection of device drivers and an API 
# for supporting realtime video capture on Linux systems.[1] 
# It supports many USB webcams, TV tuners, and related devices, 
# standardizing their output, so programmers can easily add video support to their applications.

# Video4Linux is responsible for creating V4L2 device nodes aka a device file 
# (/dev/videoX, /dev/vbiX and /dev/radioX) 
# and tracking data from these nodes. 
#     The device node creation is handled by V4L device drivers using the video_device struct 
#     (v4l2-dev.h) 
# and it can either be allocated dynamically or embedded in another larger struct.

# VS

# devio.c  --  User space communication with USB devices.
    # It allows user space programs/"drivers" to communicate directly
    # with USB devices without intervening kernel driver.


#---------------------------------------------------------



istcpdump=0

#---------------------------------------------------------


clear
echo "new new new new new new new new"
echo "new new new new new new new new"

sudo modprobe -r uvcvideo
sleep 2
sudo modprobe uvcvideo
sleep 2

example_number=${1}
device_vidpid=${2}":"${3}
device_vid=${2}
device_pid=${3}
device_file=${4}
videon=${device_file}
resolution=${5}":"${6}
width=${5}
height=${6}
fmt_index=${7}
frame_index=${8}
fps=${9}
output_dir=${10}"/"$width"x"$height
host_results=`pwd`/host/host_results
binary_path=`pwd`"/v4l2-examples-master/example-${example_number}"
root_dir=`pwd`

v4l2-ctl --device /dev/${device_file} --set-fmt-video=width=${width},height=${height},pixelformat=YUYV

echo "example number:       " ${example_number}
echo "device_vidpid:        " ${device_vidpid}
echo "device file:          " ${device_file}
echo "resolution:           " ${resolution}
echo "fmt_index:            " ${fmt_index}
echo "fps:                  " ${fps}
echo "output_dir:           " ${output_dir}
echo "binary_path:          " ${binary_path}


#output_dir:            /home/dgnet/experiment/3/c270/libuvc/1/640x480
#binary_path:           /home/dgnet/experiment/3/libuvc_debug/build

mkdir -p ${output_dir}
mkdir -p ${host_results}
rm -fR ${output_dir}/*
rm -fR ${binary_path}/out


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
rm -fR ./out
#git pull
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
echo "example-"${example_number} >> ${output_dir}/dmesg.txt
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
echo "./example-${example_number} ${device_vid} ${device_pid} /dev/${device_file} ${width} ${height} $fmt_index" >> ${output_dir}/ex_out.txt 2>&1
echo "========================" >> ${output_dir}/ex_out.txt 2>&1
echo "========================" >> ${output_dir}/ex_out.txt 2>&1

cd ${binary_path}
./example-${example_number} ${device_vid} ${device_pid} "/dev/"${device_file} ${width} ${height} $fmt_index >> ${output_dir}/ex_out.txt 2>&1
cd ${output_dir}

sleep 1
ffmpeg -f rawvideo -pix_fmt yuyv422 -s:v ${width}x${height} -r ${fps} -i ${binary_path}/out output.avi


cp ${binary_path}/out ${output_dir}


#======================================================
#======================================================

sleep 1
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


# #======================================================

# tar --exclude='*.pcap' -czf ./${videon}.tar.gz ./*

# #======================================================

sync
sync
exit 0