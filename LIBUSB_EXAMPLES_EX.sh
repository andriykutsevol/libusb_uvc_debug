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