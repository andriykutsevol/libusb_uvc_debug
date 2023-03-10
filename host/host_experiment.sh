#!/bin/bash
set -e

#---------------------------------------------------------
# NOTES:
# 1) GIT error: Your local changes to the following files would be overwritten by merge...
#    git reset --hard
#    git pull

# 2) trace_events.txt Be sure to leave a blank line at the end of the file.


#---------------------------------------------------------

# 
# 

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


auditctl -e 0
sleep 1
clear

#--------------------------------------------------

working_dir=`pwd`
framework_dir="/home/dgnet/qemu/guest/libusb_uvc_debug"

output_dir=${framework_dir}/host/host_results
mkdir -p ${output_dir}
dmesg_file=${output_dir}/dmesg_host.txt


vm_name="ubuntu20.04"

usb_device_xml=$(head -n 1 ./../../../usb_device_to_attach.txt)
#usb_device_xml="usb_device_c270_cam.xml"
#--------------------------------------------------

mkdir -p ${output_dir}
rm -f ${output_dir}/*
sync

#--------------------------------------------------


echo "=============================" >> ${dmesg_file}
echo "sudo modprobe -r uvcvideo" >> ${dmesg_file} 
echo "sudo modprobe uvcvideo" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

sleep 1
dmesg -c > /dev/null 2>&1
sudo modprobe -r uvcvideo
sleep 1
sudo modprobe uvcvideo


dmesg -c >> ${dmesg_file}

echo "=============================" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

#--------------------------------------------------



virsh start ${vm_name}

echo "Attach device (enter):"
read varname
echo "Before attach:"
virsh qemu-monitor-command ${vm_name} --hmp "info usb"
usb-devices > ${output_dir}/usb-devices_before_attach.txt
#--------------------------------------------------
#--------------------------------------------------
#ATTACH
#--------------------------------------------------
#--------------------------------------------------


#-------------------------------
# tcpdump
# modprobe usbmon
# sudo tcpdump -i usbmon2 -w ${output_dir}/tcpdump_attach.pcap &
# tcp_dump_pid=$!
# sleep 1

echo "=============================" >> ${dmesg_file} 
echo "attach-device ${vm_name}" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

sudo virsh attach-device ${vm_name} --file ./${usb_device_xml}
sleep 5

#-------------------------------
# tcpdump
# echo "kill tcp_dump_pid: $tcp_dump_pid"
# kill -9 $tcp_dump_pid
# sleep 1
#-------------------------------

dmesg -c >> ${dmesg_file}

echo "=============================" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

echo "After attach:"
virsh qemu-monitor-command ${vm_name} --hmp "info usb"
usb-devices > ${output_dir}/usb-devices_after_attach.txt


#-------------------------------
# tcpdump
# modprobe usbmon
# sudo tcpdump -i usbmon2 -w ${output_dir}/tcpdump_experiment.pcap &
# tcp_dump_pid=$!
# sleep 1

echo "NOW RUN YOUR GUEST EXPERIMENT."
echo "When it will be completed - press Enter"
echo "=============================" >> ${dmesg_file} 
echo "EXPERIMENT" >> ${dmesg_file}
echo "=============================" >> ${dmesg_file}

read varname

#-------------------------------
# tcpdump
# echo "kill tcp_dump_pid: $tcp_dump_pid"
# kill -9 $
# sleep 1
#-------------------------------

dmesg -c >> ${dmesg_file}

sudo chmod -R a+rwx ${output_dir}
sudo chmod -R a+rwx ${framework_dir}

echo "=============================" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 


echo "Detach device (enter):"
sudo virsh detach-device ${vm_name} --file ./${usb_device_xml}
sleep 3
echo "after detach"
virsh qemu-monitor-command ${vm_name} --hmp "info usb"

echo "shutdown (enter):"
#read varname
virsh shutdown ${vm_name}
sleep 5
sync
sync
sleep 5
sync
sync