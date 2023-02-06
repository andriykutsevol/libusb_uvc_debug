On my 3240 it works without adding isolation to the "grub" config file.

There is two variables to set in the usb_controller_hotplug script:

1) usb_device_xml_path
        currently it is "./device.xml".
            Which is the name of the xml file describing the device to attach.
2) vm_name
        currently it is "ubuntu20.04". 
            Which is the name of the virtual machine to which the device should be attached.



The virtual machine with the "vm_name" has already be running.

First step:

sudo virsh attach-device ${vm_name} --file ./${usb_device_xml_path}

Second step:

sudo virsh detach-device ${vm_name} --file ./${usb_device_xml_path}


