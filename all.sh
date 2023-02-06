#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


#===================================================================


./libuvc_example1_aver.sh
./libuvc_example1_google.sh

./v4l2_example5_aver.sh
./v4l2_example5_google.sh


rm -f ./ex.tar.gz
tar -czf ./ex.tar.gz --exclude='*.pcap' ./*