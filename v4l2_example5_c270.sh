#!/bin/bash
set -e

output_dir=`pwd`"/c270/v4l2-examples/5"

./v4l2_examples_ex.sh 5 1133 2085 video0 640 480 0 15 ${output_dir}


./v4l2_examples_ex.sh 5 1133 2085 video0 1280 720 0 15 ${output_dir}

