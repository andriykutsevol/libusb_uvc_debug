#!/bin/bash
set -e 

#---------------------------------------------------------
# NOTES:
# 1) Error when build: somefile.make has modification time 518 s in the future
    # This happens when you copy build outputs between two computers, and the clocks on those computers do not agree. 
    # make clean ought to force regeneration of all files using the local clock, 
    # and after that you shouldn't see the issue again... unless your computer clock is badly broken. 
        # To expand on Ben Voigt's answer:
        #     find /your/dir -type f -exec touch {} +
        #     will update the timestamp on all files in the directory. You can then make clean && make again

#---------------------------------------------------------



root_dir=`pwd`

cd ./libuvc_debug/build
rm -R ./*
cmake ..
make
