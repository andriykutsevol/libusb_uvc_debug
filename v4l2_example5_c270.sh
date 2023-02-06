#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi


output_dir=`pwd`"/c270/v4l2-examples/5"

./v4l2_examples_ex.sh 5 1133 2085 video0 640 480 0 15 ${output_dir}


./v4l2_examples_ex.sh 5 1133 2085 video0 1280 720 0 15 ${output_dir}

