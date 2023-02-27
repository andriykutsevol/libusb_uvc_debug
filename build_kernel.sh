#!/bin/bash
set -e

if [ "$EUID" -eq 0 ]
  then echo "Do not run as root"
  exit
fi


# path_to_kernel="/home/dgnet/kernel/v5.19.13-zen1_qemu_libusb_ex"
path_to_kernel="/home/dgnet/kernel"

cd ${path_to_kernel}/src/zen-kernel
# This taken from PKGBUILD
git pull https://github.com/andriykutsevol/zen-kernel.git v5.19.13-zen1_qemu_libusb_ex

echo "================================="
echo "================================="
echo "================================="


cd ${path_to_kernel}
makepkg --skipinteg --skipchecksums --skippgpcheck -ef


echo "================================="
echo "================================="
echo "================================="


sudo pacman -U linux-zen-5.19.13.zen1-1-x86_64.pkg.tar.zst
