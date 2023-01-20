#!/bin/bash


qemu_path=/home/dgnet/qemu/DGnet_ALP_PID3
qemu_src_path=${qemu_path}/src/qemu-6.0.0/



cd ${qemu_src_path}
git pull 
cd ${qemu_path}
makepkg --skipinteg --skipchecksums --skippgpcheck -ef

echo "INTALL PACKAGE:"
sudo pacman -U ./qemu-headless-6.0.0-3-x86_64.pkg.tar.zst

