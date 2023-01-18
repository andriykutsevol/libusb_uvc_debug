#!/bin/bash
set -e


path_to_kernel="/home/dgnet/kernel-v5.19.13.zen1"

cd ${path_to_kernel}/src/zen-kernel
git pull https://github.com/andriykutsevol/zen-kernel.git v5.19.13-zen1_qemu_libusb_ex

echo "================================="
echo "================================="
echo "================================="


cd ${path_to_kernel}
makepkg --skipinteg --skipchecksums --skippgpcheck -ef


echo "================================="
echo "================================="
echo "================================="



#===============================================================

# $ make M=drivers/media/usb/uvc/
#   MODPOST drivers/media/usb/uvc/Module.symvers
#   CC [M]  drivers/media/usb/uvc/uvcvideo.mod.o
#   LD [M]  drivers/media/usb/uvc/uvcvideo.ko
#   BTF [M] drivers/media/usb/uvc/uvcvideo.ko


