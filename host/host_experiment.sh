#!/bin/bash

auditctl -e 0
modprobe usbmon
sleep 1
clear
#--------------------------------------------------

rm -f /home/dgnet/experiment/guest/results/*
sync

#--------------------------------------------------


output_dir="/home/dgnet/experiment/guest/results"
dmesg_file=${output_dir}/dmesg_host.txt

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



virsh start ubuntu20.04

echo "Attach device (enter):"
read varname
echo "Before attach:"
virsh qemu-monitor-command ubuntu20.04 --hmp "info usb"
usb-devices > ${output_dir}/usb-devices_before_attach.txt
#--------------------------------------------------
#--------------------------------------------------
#ATTACH
#--------------------------------------------------
#--------------------------------------------------



sudo tcpdump -i usbmon2 -w ${output_dir}/tcpdump_attach.pcap &
tcp_dump_pid=$_
sleep 1
echo "=============================" >> ${dmesg_file} 
echo "attach-device ubuntu20.04" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

sudo virsh attach-device ubuntu20.04 --file ./usb_device_cam.xml
sleep 5

kill -9 $tcp_dump_pid
sleep 1

dmesg -c >> ${dmesg_file}

echo "=============================" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

echo "After attach:"
virsh qemu-monitor-command ubuntu20.04 --hmp "info usb"
usb-devices > ${output_dir}/usb-devices_after_attach.txt


sudo tcpdump -i usbmon2 -w ${output_dir}/tcpdump_experiment.pcap &
tcp_dump_pid=$_
sleep 1
echo "NOW RUN YOUR GUEST EXPERIMENT."
echo "When it will be completed - press Enter"
echo "=============================" >> ${dmesg_file} 
echo "EXPERIMENT" >> ${dmesg_file}
echo "=============================" >> ${dmesg_file} 



read varname

kill -9 $tcp_dump_pid
sleep 1
dmesg -c >> ${dmesg_file}

sudo chmod -R a+rwx ${output_dir}
sudo chmod -R a+rwx /home/dgnet/experiment/guest/libusb_uvc_debug_11/libusb_uvc_debug_11

echo "=============================" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 


echo "Detach device (enter):"
sudo virsh detach-device ubuntu20.04 --file ./usb_device_cam.xml
sleep 3
echo "after detach"
virsh qemu-monitor-command ubuntu20.04 --hmp "info usb"

echo "shutdown (enter):"
#read varname
virsh shutdown ubuntu20.04
sleep 5