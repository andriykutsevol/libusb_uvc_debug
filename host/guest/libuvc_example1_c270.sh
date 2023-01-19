#!/bin/bash
sudo mount -t 9p -o trans-virtio /home/dgnet/experiment/host /home/dgnet/experiment/host_mnt
cd /home/dgnet/experiment/host_mnt/libusb_uvc_debug_11/libusb_uvc_debug_11
sudo ./libuvc_example1_c270.sh 
