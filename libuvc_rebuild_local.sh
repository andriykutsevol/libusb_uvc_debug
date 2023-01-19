#!/bin/bash
set -e 

root_dir=`pwd`

cd ./libuvc_debug/build
rm -R ./*
cmake ..
make
