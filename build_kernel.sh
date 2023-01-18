#!/bin/bash
set -e


path_to_kernel="/home/dgnet/kernel"

cd ${path_to_kernel}/src/zen-kernel
git pull https://github.com/andriykutsevol/v6.0.7-zen1.git hdmi

echo "================================="
echo "================================="
echo "================================="


cd ${path_to_kernel}
makepkg --skipinteg --skipchecksums --skippgpcheck -ef


echo "================================="
echo "================================="
echo "================================="


sudo pacman -U linux-zen-6.0.7.zen1-1-x86_64.pkg.tar.zst
