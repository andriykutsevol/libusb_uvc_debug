#!/bin/bash

if [ "$EUID" -eq 0 ]
  then echo "Do not run as root"
  exit
fi



qemu_path=/home/dgnet/qemu/DGnet_ALP_PID3
qemu_src_path=${qemu_path}/src/qemu-6.0.0/



cd ${qemu_src_path}
git pull https://github.com/andriykutsevol/DGnet_Dist_PID3-my.git usbspoof_impr_spec1_working
cd ${qemu_path}
makepkg --skipinteg --skipchecksums --skippgpcheck -ef

echo "----------------------------"
echo "INTALL PACKAGE:"
echo "----------------------------"
sudo pacman -U ./qemu-headless-6.0.0-3-x86_64.pkg.tar.zst

