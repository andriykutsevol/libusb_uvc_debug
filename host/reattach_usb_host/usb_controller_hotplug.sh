#!/bin/bash
set -e

#---------------------------------------------------------

if [ "$EUID" -ne 0 ]
  then echo "Please run it as root"
  exit
fi


auditctl -e 0
sleep 1
clear

#---------------------------------------------------------

usb_device_xml_path="./device.xml"

vm_name="ubuntu20.04"

#---------------------------------------------------------


echo "============================="
echo "attach-device ${vm_name}"
echo "============================="


sudo virsh attach-device ${vm_name} --file ./${usb_device_xml_path}
sleep 10


echo "============================="
echo "============================="

echo "After attach:"
virsh qemu-monitor-command ${vm_name} --hmp "info usb"

echo "NOW RUN YOUR GUEST EXPERIMENT."
echo "When it will be completed - press Enter"

read varname



echo "============================="
echo "============================="


echo "Detach device: "
sudo virsh detach-device ${vm_name} --file ./${usb_device_xml_path}

sleep 10

echo "After detach"
virsh qemu-monitor-command ${vm_name} --hmp "info usb"


sync
sync
echo "Done."