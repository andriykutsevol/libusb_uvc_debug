#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi



# example1.c:

# uvc_stream_ctrl_t ctrl;

# /* Try to negotiate first stream profile */
# res = uvc_get_stream_ctrl_format_size(
#     devh, &ctrl, /* result stored in ctrl */
#     frame_format,
#     width, height, fps /* width, height, fps */
# );

#         uvc_query_stream_ctrl( devh, ctrl, 1, UVC_GET_MAX);
        


# res = uvc_start_streaming(devh, &ctrl, cb, (void *) 12345, 0);

#     ret = uvc_stream_open_ctrl(devh, &strmh, ctrl);

#         uvc_stream_ctrl()

#             uvc_query_stream_ctrl()

#                 !!!! ТУТ происходит выборка значений
    #                 !!!dgnet: UVCLIB: ctrl->dwMaxVideoFrameSize: 614400
    #                 !!!dgnet: UVCLIB: ctrl->dwMaxPayloadTransferSize: 3072
#                 Это происходит через:  !!! libusb_control_transfer !!!
#                 Именно он возвращает значения:
#                     ctrl->dwMaxVideoFrameSize
#                     ctrl->dwMaxPayloadTransferSize

#                 Поэтому для запуска этого шестого примера, нужно значале запустить
#                 пример 1 и там найти эти значения


                              # EXAMPLE: Format: (YUY2) 640x480 30fps
                              # !!!dgnet: THE RESULT of libusb_control_transfer (now decode following a GET transfer)
                              # !!!dgnet: UVCLIB: probe: 1
                              # !!!dgnet: UVCLIB: ctrl->bmHint: 1
                              # !!!dgnet: UVCLIB: ctrl->bFormatIndex: 1
                              # !!!dgnet: UVCLIB: ctrl->bFrameIndex: 1
                              # !!!dgnet: UVCLIB: ctrl->dwFrameInterval: 333333
                              # !!!dgnet: UVCLIB: ctrl->wKeyFrameRate: 0
                              # !!!dgnet: UVCLIB: ctrl->wPFrameRate: 0
                              # !!!dgnet: UVCLIB: ctrl->wCompQuality: 2000
                              # !!!dgnet: UVCLIB: ctrl->wCompWindowSize: 0
                              # !!!dgnet: UVCLIB: ctrl->wDelay: 0
                              # !!!dgnet: UVCLIB: ctrl->dwMaxVideoFrameSize: 614400
                              # !!!dgnet: UVCLIB: ctrl->dwMaxPayloadTransferSize: 3060
                              # !!!dgnet: UVCLIB: ctrl->bInterfaceNumber: 1


#     ret = uvc_stream_start(strmh, cb, user_ptr, flags);

#         /* Go through the altsettings and find one whose packets are at least
#         * as big as our format's maximum per-packet usage. Assume that the
#         * packet sizes are increasing. */

#             if (endpoint->bEndpointAddress == format_desc->parent->bEndpointAddress) {
#               endpoint_bytes_per_packet = endpoint->wMaxPacketSize;
#             // wMaxPacketSize: [unused:2 (multiplier-1):3 size:11]
#             endpoint_bytes_per_packet = (endpoint_bytes_per_packet & 0x07ff) *
#               (((endpoint_bytes_per_packet >> 11) & 3) + 1);
#             break;

#         packets_per_transfer = (ctrl->dwMaxVideoFrameSize +
#                                 endpoint_bytes_per_packet - 1) / endpoint_bytes_per_packet;


#         total_transfer_size = packets_per_transfer * endpoint_bytes_per_packet;

#         libusb_fill_iso_transfer(
#             transfer, strmh->devh->usb_devh, format_desc->parent->bEndpointAddress,
#             strmh->transfer_bufs[transfer_id],
#             total_transfer_size, packets_per_transfer, _uvc_stream_callback, (void*) strmh, 5000); 




#====================================================
#====================================================
#====================================================
#c270
#====================================================
#====================================================
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
# 640x480
#====================================================

# Руками устанавливаем:

# 0)
      # uint8_t contol_bInterfaceNumber = 0;          # Нулевой интерфейс всегда контрольный.

# 1)
      # uint8_t bEndpointAddress = 135;

            # Это берется из lsuvc:

                  # ======================================
                  # END VIDEO CONTROL PARSING.
                  # ======================================
                  # ------------------------
                  # ENDPOINT: endpoint_idx 0, ALTSETTING(if_desc): 0, INTERFACE: 0
                  # ep_desc->bLength: 7
                  # ep_desc->bDescriptorType: 5
                  # ep_desc->bEndpointAddress: 135

            # Это эндпоинт нулевого интерфейса.

            # А так же это можно взять из:
            #       !!!dgnet: UVCLIB: uvc_open_internal() 6: libusb_fill_interrupt_transfer():  bEndpointAddress           




# 2)
# uint16_t bcdUVC = 0x100;
      # lsuvc: bcdUVC = SW_TO_SHORT(&buffer_ci[3]): 100
# uint8_t probe = 0;
# enum uvc_req_code req = UVC_SET_CUR;


# 3)
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

    # Последний
    # !!!dgnet: THE RESULT of libusb_control_transfer (now decode following a GET transfer)
    # !!!dgnet: UVCLIB: probe: 1
    # !!!dgnet: UVCLIB: ctrl->bmHint: 1
    # !!!dgnet: UVCLIB: ctrl->bFormatIndex: 1
    # !!!dgnet: UVCLIB: ctrl->bFrameIndex: 1
    # !!!dgnet: UVCLIB: ctrl->dwFrameInterval: 333333
    # !!!dgnet: UVCLIB: ctrl->wKeyFrameRate: 0
    # !!!dgnet: UVCLIB: ctrl->wPFrameRate: 0
    # !!!dgnet: UVCLIB: ctrl->wCompQuality: 2000
    # !!!dgnet: UVCLIB: ctrl->wCompWindowSize: 0
    # !!!dgnet: UVCLIB: ctrl->wDelay: 0
    # !!!dgnet: UVCLIB: ctrl->dwMaxVideoFrameSize: 614400
    # !!!dgnet: UVCLIB: ctrl->dwMaxPayloadTransferSize: 3060
    # !!!dgnet: UVCLIB: ctrl->bInterfaceNumber: 1



# 4)
#           size_t len;
#           if (bcdUVC >= 0x0110)
#             len = 34;
#           else
#             len = 26; 



# 5)
        # /* Number of packets per transfer */
        # size_t packets_per_transfer = 32;
        # /* Size of packet transferable from the chosen endpoint */
        # size_t endpoint_bytes_per_packet = 3060;


# 5)

        # uint8_t bAlternateSetting = 11;
              # !!!dgnet: UVCLIB: -----START ISOCHRONOUS TRANSFER-----
              # ...
              # !!!dgnet: UVCLIB: libusb_set_interface_alt_setting(): altsetting->bAlternateSetting 11
              # ret = libusb_set_interface_alt_setting(strmh->devh->usb_devh,
              #                                       altsetting->bInterfaceNumber,
              #                                       altsetting->bAlternateSetting); 

        # uint8_t bEndpointAddres = 129;
        # size_t total_transfer_size = 97920;
              # !!!dgnet: UVCLIB: total_transfer_size: 97920
                  # total_transfer_size = packets_per_transfer * endpoint_bytes_per_packet;



# !!!dgnet: UVCLIB: libusb_set_interface_alt_setting(): altsetting->bInterfaceNumber 1



#=====================================================




# uint8_t contol_bInterfaceNumber_mysetting = 0;

# uint8_t bEndpointAddress_mysetting = 135;

# uint16_t bcdUVC_mysetting = 0x100; // 256

# uint8_t bInterfaceNumber_mysetting = 1;
# uint16_t bmHint_mysetting = 1;
# uint8_t bFormatIndex_mysetting = 1;
# uint8_t bFrameIndex_mysetting = 1;
# uint32_t dwFrameInterval_mysetting = 333333;
# uint16_t wKeyFrameRate_mysetting = 0;
# uint16_t wPFrameRate_mysetting = 0;
# uint16_t wCompQuality_mysetting = 200;
# uint16_t wCompWindowSize_mysetting = 0;
# uint16_t wDelay_mysetting = 0;
# uint32_t dwMaxVideoFrameSize_mysetting = 614400;
# uint32_t dwMaxPayloadTransferSize_mysetting = 3060;

# size_t endpoint_bytes_per_packet_mysetting = 3060;

# uint8_t bAlternateSetting_mysetting = 11;

# uint8_t bEndpointAddres_mysetting = 129;

# size_t total_transfer_size_mysetting = 97920;




#------------------------------------------------------


contol_bInterfaceNumber_mysetting=0;
bEndpointAddress_mysetting=135;
bcdUVC_mysetting=256  # 0x100;


bInterfaceNumber_mysetting=1;
bmHint_mysetting=1;
bFormatIndex_mysetting=1;
bFrameIndex_mysetting=1;
dwFrameInterval_mysetting=333333;
wKeyFrameRate_mysetting=0;
wPFrameRate_mysetting=0;
wCompQuality_mysetting=200;
wCompWindowSize_mysetting=0;
wDelay_mysetting=0;
dwMaxVideoFrameSize_mysetting=614400;
dwMaxPayloadTransferSize_mysetting=3060;


endpoint_bytes_per_packet_mysetting=3060;
bAlternateSetting_mysetting=11;
bEndpointAddres_mysetting=129;
total_transfer_size_mysetting=97920;

#------------------------------------------------------
fps=15
#------------------------------------------------------

ex_num=6
output_dir=`pwd`"/c270/libusb/${ex_num}"

./LIBUSB_EXAMPLES_EX.sh ${ex_num} 1133 2085 video0 640 480 \
${contol_bInterfaceNumber_mysetting} \
${bEndpointAddress_mysetting} \
${bcdUVC_mysetting} \
${bInterfaceNumber_mysetting} \
${bmHint_mysetting} \
${bFormatIndex_mysetting} \
${bFrameIndex_mysetting} \
${dwFrameInterval_mysetting} \
${wKeyFrameRate_mysetting} \
${wPFrameRate_mysetting} \
${wCompQuality_mysetting} \
${wCompWindowSize_mysetting} \
${wDelay_mysetting} \
${dwMaxVideoFrameSize_mysetting} \
${dwMaxPayloadTransferSize_mysetting} \
${endpoint_bytes_per_packet_mysetting} \
${bAlternateSetting_mysetting} \
${bEndpointAddres_mysetting} \
${total_transfer_size_mysetting} \
${fps} \
${output_dir}




#------------------------------------------------------

#-----------------------
#Make Slice
#-----------------------
working_dir=`pwd`
cd ${working_dir}

dev_name="c270"
slices_dir=${working_dir}/slices
#sudo rm -rf ${slices_dir}/*
sudo rm -rf {dev_name}_slice.tar.gz
mkdir -p ${slices_dir}


./make_slice.sh ${working_dir} ${dev_name}
cp ./${dev_name}_slice.tar.gz ${slices_dir}/
rm -rf ./${dev_name}_slice.tar.gz
rm -rf ./${dev_name}_slice