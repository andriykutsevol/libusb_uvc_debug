#!/bin/bash
#
# usb2sys - find lsusb device in /sys file system
#

# lsusb | cut -d ' ' -f 6 | xargs -L 1 ./usb2sys

die()
{
    echo "$@"
    exit 1
}

[[ $# -lt 1 ]] && die "need vendor and product ids (from lsusb) as dddd:dddd"

vendor=${1%:*}
product=${1##*:}

sys=/sys/bus/usb/devices/
cd $sys

for d in *; do
    path=$sys$d
    if [ -f $path/idProduct ]; then
      prod=$( cat $path/idProduct )
      vend=$( cat $path/idVendor )

      if [ $prod = $product -a $vend = $vendor ]; then
        echo vend = $vend
        echo prod = $prod
        echo /sys device is $path
      fi
    fi
done
