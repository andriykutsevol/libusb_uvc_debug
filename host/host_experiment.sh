#!/bin/bash

auditctl -e 0
modprobe usbmon
sleep 1
clear

#--------------------------------------------------

guest_dir="/home/dgnet/qemu"
output_dir=${guest_dir}"/results"
framework_dir=${guest_dir}/libusb_uvc_debug
dmesg_file=${output_dir}/dmesg_host.txt


vm_name="ubuntu20.04"
#--------------------------------------------------

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



sudo tcpdump -i usbmon2 -w ${output_dir}/tcpdump_attach.pcap &
tcp_dump_pid=$_
sleep 1
echo "=============================" >> ${dmesg_file} 
echo "attach-device ${vm_name}" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

sudo virsh attach-device ${vm_name} --file ./usb_device_cam.xml
sleep 5

kill -9 $tcp_dump_pid
sleep 1

dmesg -c >> ${dmesg_file}

echo "=============================" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 

echo "After attach:"
virsh qemu-monitor-command ${vm_name} --hmp "info usb"
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
sudo chmod -R a+rwx ${framework_dir}

echo "=============================" >> ${dmesg_file} 
echo "=============================" >> ${dmesg_file} 


echo "Detach device (enter):"
sudo virsh detach-device ${vm_name} --file ./usb_device_cam.xml
sleep 3
echo "after detach"
virsh qemu-monitor-command ${vm_name} --hmp "info usb"

echo "shutdown (enter):"
#read varname
virsh shutdown ${vm_name}
sleep 5