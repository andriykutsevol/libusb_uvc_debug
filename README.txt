git clone https://github.com/andriykutsevol/libusb_uvc_debug.git

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