#!/bin/bash
sync
sync
sync
sudo mount -t 9p -o trans-virtio /home/dgnet/experiment/host /home/dgnet/experiment/host_mnt
sync
sync
sync
sleep 2
#-----------------------------------------
cd ./host_mnt/libusb_uvc_debug
sudo ./v4l2_example5_c270.sh
#-----------------------------------------
sync
sync
sync
cd /home/dgnet/experiment/
sync
sync
sync
sleep 2 
sudo umount /home/dgnet/experiment/host_mnt
sleep 2
sudo umount /home/dgnet/experiment/host_mnt
sleep 2
sudo umount /home/dgnet/experiment/host_mnt
sync
sync
sync
sleep 2
echo 'done'