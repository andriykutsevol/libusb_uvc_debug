#kernel
# sudo pacman -S base-devel cmake bc kmod libelf pahole cpio perl tar xz xmlto python-sphinx python-sphinx_rtd_theme graphviz imagemagick git texlive-latexextra
#qemu
# sudo pacman -S tcpdump base-devel cmake spice-protocol ceph libiscsi glusterfs python-sphinx xfsprogs ninja virglrenderer brltty pahole --overwrite "*"
# sudo pacman -S ffmpeg v4l-utils guvcview

makepkg --skipinteg --skipchecksums --skippgpcheck -o
makepkg --skipinteg --skipchecksums --skippgpcheck -ef



git clone https://github.com/andriykutsevol/libusb_uvc_debug.git

----------------------------------

cd ./libuvc_debug/
mkdir ./Build
cd ./build
make

----------------------------------

./libusb_build.sh

----------------------------------

cd v4l2-examples-master/example-5
make


----------------------------------

1) We are using “Virtual Machine Manager” (VM Manager).

2) Install the guest OS as usual.

3) In the VM Manager set the “Controller USB” Model to “USB 3”          !!!

4) In the MV Manager go to “Overview” and edit XML file.

4.1) Set the first line to:

<domain xmlns:qemu="http://libvirt.org/schemas/domain/qemu/1.0" type="kvm">


----------------------------------


<domain xmlns:qemu="http://libvirt.org/schemas/domain/qemu/1.0" type="kvm">
...
  <qemu:commandline>
    <qemu:arg value="-D"/>
    <qemu:arg value="/home/dgnet/qemu/guest/libusb_uvc_debug/host/host_results/D_output.txt"/>
    <qemu:arg value="--trace"/>
    <qemu:arg value="events=/home/dgnet/qemu/guest/libusb_uvc_debug/host/trace_events.txt"/>
  </qemu:commandline>

</qemu:commandline>


----------------------------------

<domain xmlns:qemu="http://libvirt.org/schemas/domain/qemu/1.0" type="kvm">
...
<qemu:commandline>
    <qemu:arg value='--usbspoof'/>
    <qemu:arg value='from=07ca:1113,to=046d:0876'/>
    <qemu:arg value='--usbspoof'/>
    <qemu:arg value='from=17e9:ff00,to=17e9:ff13'/>
  </qemu:commandline>

----------------------------------



Among others there is two important pure "libusb" utilities:

1) libusb_uvc_debug_11/libusb_examples/1/example.c
It is a tool for unwrapping the descriptors hierarchy for UVC devices.
It is used to get the required parameters for the our second utility.

2) libusb_uvc_debug_11/libusb_examples/6/example.c
This second utility get the video stream from a UVC device.
In contrast to the "libuvc" it does not handle all UVC stuff automatically.
But we do not need that. We need to research the device manually as much as possible.


So the workflow to research some UVC devices is as follows:

We run the first utility to get the required parameters,
Then apply some calculations to them.
Then put our results into the second utility to get a video stream.

The main idea is to exclude as much abstraction layers as possible,
and use the "libusb" only.


From the GitHub about libusb:
https://github.com/libusb/libusb
"""
libusb is a library for USB device access from Linux, macOS, Windows,
OpenBSD/NetBSD, Haiku, Solaris userspace, and WebAssembly via WebUSB.
"""

The "libusb"

From the "https://libusb.sourceforge.io/api-1.0/" we see that:
"""
libusb is an open source library that allows you to communicate with USB devices from user space. For more info, see the libusb homepage.
"""