#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi



#---------------------------------------------------------
# NOTES:


#---------------------------------------------------------

example_number=${1}
device_vidpid=${2}":"${3}
device_vid=${2}
device_pid=${3}
device_file=${4}
videon=${device_file}
resolution=${5}":"${6}
width=${5}
height=${6}


echo "example number:       " ${example_number}
echo "device_vidpid:        " ${device_vidpid}
echo "device file:          " ${device_file}
echo "resolution:           " ${resolution}