#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi



#====================================================
#c270
#====================================================

# lsuvc
# Guvcview tool has YUYV:
    # 1) 640 480
    # 2) 160 120
    # 3) 176 144
    # 4) 320 176
    # 5) 320 240
    # 6) 352 288
    # 7) 432 240
    # 8) 544 288
    # 9) 640 360
    # 10) 752 416
    # 11) 800 448
    # 12) 800 600
    # 13) 864 480
    # 14) 960 544
    # 15) 960 720
    # 16) 1024 576
    # 17) 1184 656
    # 18) 1280 720
    # 19) 1280 960

# Guvcview tool has MJPEG:


# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 480 1 0 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 160 120 1 1 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 176 144 1 2 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 320 176 1 3 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 320 240 1 4 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 352 288 1 5 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 432 240 1 6 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 544 288 1 7 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 360 1 8 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 752 416 1 9 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 800 448 1 10 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 800 600 1 11 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 864 480 1 12 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 960 544 1 13 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 960 720 1 14 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1024 576 1 15 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1184 656 1 16 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1280 720 1 17 15 ${output_dir}
# ./LIBUVC_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 1280 960 1 18 15 ${output_dir}




# --------EXTRA INTERFACE :2: ALTSETTING(if_desc): 0, INTERFACE: 1--------
# block_size: 50
# buffer_left: 1803
# descriptor_subtype: 5
# UVC_VS_FRAME_UNCOMPRESSED: 0                                                                !!! This means that it is a YUYV format
# UVC_VS_FRAME_MJPEG: 0
# uvc_parse_vs_frame_uncompressed(stream_if, block, block_size);
# bDescriptorSubtype = buffer[2]: 5
# bFrameIndex = buffer[3]: 1
# bmCapabilities = buffer[4]: 1
# wWidth = buffer[5] + (buffer[6] << 8): 640
# wHeight = buffer[7] + (buffer[8] << 8): 480
# dwMinBitRate = DW_TO_INT(&buffer[9]): 24576000
# dwMaxBitRate = DW_TO_INT(&buffer[13]): 147456000
# dwMaxVideoFrameBufferSize = DW_TO_INT(&buffer[17]): 614400
# dwDefaultFrameInterval = DW_TO_INT(&buffer[21]): 333333
# bFrameIntervalType = buffer[25]: 6
# intervals[0]: 333333
# intervals[1]: 400000
# intervals[2]: 500000
# intervals[3]: 666666
# intervals[4]: 1000000
# intervals[5]: 2000000
# intervals[6]: 0





# --------EXTRA INTERFACE :23: ALTSETTING(if_desc): 0, INTERFACE: 1--------
# block_size: 50
# buffer_left: 956
# descriptor_subtype: 7
# UVC_VS_FRAME_MJPEG: 19
# uvc_parse_vs_frame_uncompressed(stream_if, block, block_size);
# bDescriptorSubtype = buffer[2]: 7
# bFrameIndex = buffer[3]: 1
# bmCapabilities = buffer[4]: 1
# wWidth = buffer[5] + (buffer[6] << 8): 640
# wHeight = buffer[7] + (buffer[8] << 8): 480
# dwMinBitRate = DW_TO_INT(&buffer[9]): 24576000
# dwMaxBitRate = DW_TO_INT(&buffer[13]): 147456000
# dwMaxVideoFrameBufferSize = DW_TO_INT(&buffer[17]): 614400
# dwDefaultFrameInterval = DW_TO_INT(&buffer[21]): 333333
# bFrameIntervalType = buffer[25]: 6
# intervals[0]: 333333
# intervals[1]: 400000
# intervals[2]: 500000
# intervals[3]: 666666
# intervals[4]: 1000000
# intervals[5]: 2000000
# intervals[6]: 0



#====================================================





# uint8_t bEndpointAddress = 135;

# uint16_t bcdUVC = 0x100;
# uint8_t probe = 0;
# enum uvc_req_code req = UVC_SET_CUR;



# strmh->cur_ctrl.bInterfaceNumber = 1;
# strmh->cur_ctrl.bmHint = 1;
# strmh->cur_ctrl.bFormatIndex = 1;
# strmh->cur_ctrl.bFrameIndex = 1;
# strmh->cur_ctrl.dwFrameInterval = 333333;
# strmh->cur_ctrl.wKeyFrameRate = 0;
# strmh->cur_ctrl.wPFrameRate = 0;
# strmh->cur_ctrl.wCompQuality = 200;
# strmh->cur_ctrl.wCompWindowSize = 0;
# strmh->cur_ctrl.wDelay = 0;
# strmh->cur_ctrl.dwMaxVideoFrameSize = 614400;                         // Эту величину, мы видим где найти, она есть выше.
# strmh->cur_ctrl.dwMaxPayloadTransferSize = 3060;                      // А эту где взять - ее нужно посчитать:



#           size_t len;
#           if (bcdUVC >= 0x0110)
#             len = 34;
#           else
#             len = 26; 








#=====================================================







