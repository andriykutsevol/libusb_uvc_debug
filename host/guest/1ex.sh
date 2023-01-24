#!/bin/bash
sync
sync
sync
sudo mount -t 9p -o trans-virtio /home/dgnet/experiment/host /home/dgnet/experiment/host_mnt
sync
sync
sync
sleep 2
cd ./host_mnt/libusb_uvc_debug
sudo ./libuvc_example1_rally.sh 
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

