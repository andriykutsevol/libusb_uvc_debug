#!/bin/bash
mount -t 9p -o trans-virtio /home/dgnet/experiment/host /home/dgnet/experiment/host_mnt
cd /home/dgnet/experiment/host_mnt/libusb_uvc_debug_11/libusb_uvc_debug_11
./v4l2_example5_c270.sh