
// sudo ./example 1133 2085     // c270
// sudo ./example 10549 6        // megawell

// ffmpeg -f rawvideo -pix_fmt yuyv422 -s:v ${width}x${height} -r ${fps} -i ${binary_path}/out ${output_dir}/output.avi
// ffmpeg -f rawvideo -pix_fmt yuyv422 -s:v 640x480 -r 15 -i ./out.yuv ./output.avi

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>  // for strtol
#include <errno.h>


#include <pthread.h>
#include <signal.h>

//================================================================
//================================================================

uint8_t bInterfaceNumber_contol_mysetting;        //(+)

uint8_t bEndpointAddress_interrupt_mysetting;     //(+)

uint16_t bcdUVC_mysetting;                        //(+)

uint8_t bInterfaceNumber_streaming_mysetting;     //(+)
uint16_t bmHint_mysetting;                        //(+)
uint8_t bFormatIndex_mysetting;                   //(+)
uint8_t bFrameIndex_mysetting;                    //(+)
uint32_t dwFrameInterval_mysetting;               //(+)
uint16_t wKeyFrameRate_mysetting;                 //(+)
uint16_t wPFrameRate_mysetting;                   //(+)
uint16_t wCompQuality_mysetting;                  //(+)
uint16_t wCompWindowSize_mysetting;               //(+)
uint16_t wDelay_mysetting;                        //(+)
uint32_t dwMaxVideoFrameSize_mysetting;           //(+)
uint32_t dwMaxPayloadTransferSize_mysetting;      //(+)

size_t endpoint_bytes_per_packet_mysetting;       //(+)

uint8_t bAlternateSetting_mysetting;              //(+)

uint8_t bEndpointAddress_iso_mysetting;           //(+)

size_t total_transfer_size_mysetting;             //(+)


//================================================================
//================================================================

/** Converts an unaligned four-byte little-endian integer into an int32 */
#define DW_TO_INT(p) ((p)[0] | ((p)[1] << 8) | ((p)[2] << 16) | ((p)[3] << 24))
/** Converts an unaligned two-byte little-endian integer into an int16 */
#define SW_TO_SHORT(p) ((p)[0] | ((p)[1] << 8))
/** Converts an int16 into an unaligned two-byte little-endian integer */
#define SHORT_TO_SW(s, p) \
  (p)[0] = (s); \
  (p)[1] = (s) >> 8;
/** Converts an int32 into an unaligned four-byte little-endian integer */
#define INT_TO_DW(i, p) \
  (p)[0] = (i); \
  (p)[1] = (i) >> 8; \
  (p)[2] = (i) >> 16; \
  (p)[3] = (i) >> 24;

//================================================================
//================================================================





typedef enum uvc_error {
  /** Success (no error) */
  UVC_SUCCESS = 0,
  /** Input/output error */
  UVC_ERROR_IO = -1,
  /** Invalid parameter */
  UVC_ERROR_INVALID_PARAM = -2,
  /** Access denied */
  UVC_ERROR_ACCESS = -3,
  /** No such device */
  UVC_ERROR_NO_DEVICE = -4,
  /** Entity not found */
  UVC_ERROR_NOT_FOUND = -5,
  /** Resource busy */
  UVC_ERROR_BUSY = -6,
  /** Operation timed out */
  UVC_ERROR_TIMEOUT = -7,
  /** Overflow */
  UVC_ERROR_OVERFLOW = -8,
  /** Pipe error */
  UVC_ERROR_PIPE = -9,
  /** System call interrupted */
  UVC_ERROR_INTERRUPTED = -10,
  /** Insufficient memory */
  UVC_ERROR_NO_MEM = -11,
  /** Operation not supported */
  UVC_ERROR_NOT_SUPPORTED = -12,
  /** Device is not UVC-compliant */
  UVC_ERROR_INVALID_DEVICE = -50,
  /** Mode not supported */
  UVC_ERROR_INVALID_MODE = -51,
  /** Resource has a callback (can't use polling and async) */
  UVC_ERROR_CALLBACK_EXISTS = -52,
  /** Undefined error */
  UVC_ERROR_OTHER = -99
} uvc_error_t;



enum uvc_req_code {
  UVC_RC_UNDEFINED = 0x00,
  UVC_SET_CUR = 0x01,
  UVC_GET_CUR = 0x81,
  UVC_GET_MIN = 0x82,
  UVC_GET_MAX = 0x83,
  UVC_GET_RES = 0x84,
  UVC_GET_LEN = 0x85,
  UVC_GET_INFO = 0x86,
  UVC_GET_DEF = 0x87
};




/** VideoStreaming interface control selector (A.9.7) */
enum uvc_vs_ctrl_selector {
  UVC_VS_CONTROL_UNDEFINED = 0x00,
  UVC_VS_PROBE_CONTROL = 0x01,
  UVC_VS_COMMIT_CONTROL = 0x02,
  UVC_VS_STILL_PROBE_CONTROL = 0x03,
  UVC_VS_STILL_COMMIT_CONTROL = 0x04,
  UVC_VS_STILL_IMAGE_TRIGGER_CONTROL = 0x05,
  UVC_VS_STREAM_ERROR_CODE_CONTROL = 0x06,
  UVC_VS_GENERATE_KEY_FRAME_CONTROL = 0x07,
  UVC_VS_UPDATE_FRAME_SEGMENT_CONTROL = 0x08,
  UVC_VS_SYNC_DELAY_CONTROL = 0x09
};



typedef struct uvc_stream_ctrl {
  
  uint8_t bInterfaceNumber;
  uint16_t bmHint;
  uint8_t bFormatIndex;
  uint8_t bFrameIndex;
  uint32_t dwFrameInterval;
  uint16_t wKeyFrameRate;
  uint16_t wPFrameRate;
  uint16_t wCompQuality;
  uint16_t wCompWindowSize;
  uint16_t wDelay;
  uint32_t dwMaxVideoFrameSize;
  uint32_t dwMaxPayloadTransferSize;

  uint32_t dwClockFrequency;
  uint8_t bmFramingInfo;
  uint8_t bPreferredVersion;
  uint8_t bMinVersion;
  uint8_t bMaxVersion;

} uvc_stream_ctrl_t;


//================================================================
//================================================================

pthread_t handler_thread;
int kill_handler_thread = 0;

#define LIBUVC_NUM_TRANSFER_BUFS 100
#define LIBUVC_XFER_META_BUF_SIZE ( 4 * 1024 )


/** An image frame received from the UVC device
 * @ingroup streaming
 */
typedef struct uvc_frame {

  /** Image data for this frame */
  void *data;
  /** Size of image data buffer */
  size_t data_bytes;

  /** Estimate of system time when the device started capturing the image */
  struct timeval capture_time;
  /** Estimate of system time when the device finished receiving the image */
  struct timespec capture_time_finished;  

  /** Metadata for this frame if available */
  void *metadata;
  /** Size of metadata buffer */
  size_t metadata_bytes;

  uint32_t sequence;

} uvc_frame_t;



struct uvc_stream_handle {


  /** if true, stream is running (streaming video to host) */
  uint8_t running;

  struct uvc_stream_ctrl cur_ctrl;

  /* listeners may only access hold*, and only when holding a
   * lock on cb_mutex (probably signaled with cb_cond) */
  uint8_t fid;
  uint32_t seq, hold_seq;
  uint32_t pts, hold_pts;
  uint32_t last_scr, hold_last_scr;
  size_t got_bytes, hold_bytes;
  uint8_t *outbuf, *holdbuf;

  pthread_mutex_t cb_mutex;
  pthread_cond_t cb_cond;

  //pthread_t cb_thread;

  uint32_t last_polled_seq;
  void *user_ptr;


  struct libusb_transfer *transfers[LIBUVC_NUM_TRANSFER_BUFS];
  uint8_t *transfer_bufs[LIBUVC_NUM_TRANSFER_BUFS];


  struct uvc_frame frame;
  struct timespec capture_time_finished;

  /* raw metadata buffer if available */
  uint8_t *meta_outbuf, *meta_holdbuf;
  size_t meta_got_bytes, meta_hold_bytes;


};

typedef struct uvc_stream_handle uvc_stream_handle_t;



//================================================================
//================================================================


void _uvc_swap_buffers(uvc_stream_handle_t *strmh) {
  printf("UVCLIB: START: _uvc_swap_buffers()\n");

  uint8_t *tmp_buf;

  pthread_mutex_lock(&strmh->cb_mutex);

  (void)clock_gettime(CLOCK_MONOTONIC, &strmh->capture_time_finished);

  /* swap the buffers */
  tmp_buf = strmh->holdbuf;
  strmh->hold_bytes = strmh->got_bytes;
  strmh->holdbuf = strmh->outbuf;
  strmh->outbuf = tmp_buf;
  strmh->hold_last_scr = strmh->last_scr;
  strmh->hold_pts = strmh->pts;
  strmh->hold_seq = strmh->seq;
  
  /* swap metadata buffer */
  tmp_buf = strmh->meta_holdbuf;
  strmh->meta_holdbuf = strmh->meta_outbuf;
  strmh->meta_outbuf = tmp_buf;
  strmh->meta_hold_bytes = strmh->meta_got_bytes;

  pthread_cond_broadcast(&strmh->cb_cond);
  pthread_mutex_unlock(&strmh->cb_mutex);

  strmh->seq++;
  strmh->got_bytes = 0;
  strmh->meta_got_bytes = 0;
  strmh->last_scr = 0;
  strmh->pts = 0;


  printf("UVCLIB: STOP: _uvc_swap_buffers()\n");
}



//================================================================
//================================================================


// _uvc_process_payload(strmh, pktbuf, pkt->actual_length);
void _uvc_process_payload(uvc_stream_handle_t *strmh, uint8_t *payload, size_t payload_len) {
  printf("UVCLIB: START: _uvc_process_payload()\n");

  size_t header_len;
  uint8_t header_info;
  size_t data_len;

  /* ignore empty payload transfers */
  if (payload_len == 0)
    return;  

  header_len = payload[0];

  if (header_len > payload_len) {
    printf("UVCLIB: ERROR: _uvc_process_payload(): bogus packet: actual_len=%zd, header_len=%zd\n", payload_len, header_len);
    return;
  }

  data_len = payload_len - header_len;
  printf("UVCLIB: START: _uvc_process_payload 1(): data_len: %d\n", data_len);

  if (header_len < 2) {
    header_info = 0;
  } else {

    /** @todo we should be checking the end-of-header bit */
    size_t variable_offset = 2;

    header_info = payload[1];

    if (header_info & 0x40) {
      printf("UVCLIB: ERROR: _uvc_process_payload(): bad packet: error bit set\n");
      return;
    }

    if (strmh->fid != (header_info & 1) && strmh->got_bytes != 0) {
      /* The frame ID bit was flipped, but we have image data sitting
         around from prior transfers. This means the camera didn't send
         an EOF for the last transfer of the previous frame. */
      printf("UVCLIB: START: _uvc_process_payload(): 2\n");   
      _uvc_swap_buffers(strmh);
    }

    strmh->fid = header_info & 1;

    if (header_info & (1 << 2)) {
      strmh->pts = DW_TO_INT(payload + variable_offset);
      variable_offset += 4;
    }

    if (header_info & (1 << 3)) {
      /** @todo read the SOF token counter */
      strmh->last_scr = DW_TO_INT(payload + variable_offset);
      variable_offset += 6;
    }

    if (header_len > variable_offset) {
        printf("UVCLIB: START: _uvc_process_payload() 3: Metadata is attached to header\n");
        // Metadata is attached to header
        size_t meta_len = header_len - variable_offset;
        if (strmh->meta_got_bytes + meta_len > LIBUVC_XFER_META_BUF_SIZE)
          meta_len = LIBUVC_XFER_META_BUF_SIZE - strmh->meta_got_bytes; /* Avoid overflow. */
        memcpy(strmh->meta_outbuf + strmh->meta_got_bytes, payload + variable_offset, meta_len);
        strmh->meta_got_bytes += meta_len;
    }
  }


  if (data_len > 0) {
    if (strmh->got_bytes + data_len > strmh->cur_ctrl.dwMaxVideoFrameSize)
      data_len = strmh->cur_ctrl.dwMaxVideoFrameSize - strmh->got_bytes; /* Avoid overflow. */
    memcpy(strmh->outbuf + strmh->got_bytes, payload + header_len, data_len);
    strmh->got_bytes += data_len;
    if (header_info & (1 << 1) || strmh->got_bytes == strmh->cur_ctrl.dwMaxVideoFrameSize) {
      /* The EOF bit is set, so publish the complete frame */
      printf("UVCLIB: START: _uvc_process_payload(): 4\n");
      _uvc_swap_buffers(strmh);
    }
  }



  printf("UVCLIB: END: _uvc_process_payload()\n");

}



// void uvc_process_control_status(uvc_device_handle_t *devh, unsigned char *data, int len) {
//   enum uvc_status_class status_class;
//   uint8_t originator = 0, selector = 0, event = 0;
//   enum uvc_status_attribute attribute = UVC_STATUS_ATTRIBUTE_UNKNOWN;
//   void *content = NULL;
//   size_t content_len = 0;
//   int found_entity = 0;
//   struct uvc_input_terminal *input_terminal;
//   struct uvc_processing_unit *processing_unit;

//   UVC_ENTER();

//   if (len < 5) {
//     UVC_DEBUG("Short read of VideoControl status update (%d bytes)", len);
//     UVC_EXIT_VOID();
//     return;
//   }

//   originator = data[1];
//   event = data[2];
//   selector = data[3];

//   if (originator == 0) {
//     UVC_DEBUG("Unhandled update from VC interface");
//     UVC_EXIT_VOID();
//     return;  /* @todo VideoControl virtual entity interface updates */
//   }

//   if (event != 0) {
//     UVC_DEBUG("Unhandled VC event %d", (int) event);
//     UVC_EXIT_VOID();
//     return;
//   }

//   /* printf("bSelector: %d\n", selector); */

//   DL_FOREACH(devh->info->ctrl_if.input_term_descs, input_terminal) {
//     if (input_terminal->bTerminalID == originator) {
//       status_class = UVC_STATUS_CLASS_CONTROL_CAMERA;
//       found_entity = 1;
//       break;
//     }
//   }

//   if (!found_entity) {
//     DL_FOREACH(devh->info->ctrl_if.processing_unit_descs, processing_unit) {
//       if (processing_unit->bUnitID == originator) {
//         status_class = UVC_STATUS_CLASS_CONTROL_PROCESSING;
//         found_entity = 1;
//         break;
//       }
//     }
//   }

//   if (!found_entity) {
//     UVC_DEBUG("Got status update for unknown VideoControl entity %d",
//   (int) originator);
//     UVC_EXIT_VOID();
//     return;
//   }

//   attribute = data[4];
//   content = data + 5;
//   content_len = len - 5;

//   UVC_DEBUG("Event: class=%d, event=%d, selector=%d, attribute=%d, content_len=%zd",
//     status_class, event, selector, attribute, content_len);

//   if(devh->status_cb) {
//     UVC_DEBUG("Running user-supplied status callback");
//     devh->status_cb(status_class,
//                     event,
//                     selector,
//                     attribute,
//                     content, content_len,
//                     devh->status_user_ptr);
//   }
  
//   UVC_EXIT_VOID();
// }


//----------------------------------------------------------


// void uvc_process_streaming_status(uvc_device_handle_t *devh, unsigned char *data, int len) {
  
//   UVC_ENTER();

//   if (len < 3) {
//     UVC_DEBUG("Invalid streaming status event received.\n");
//     UVC_EXIT_VOID();
//     return;
//   }

//   if (data[2] == 0) {
//     if (len < 4) {
//       UVC_DEBUG("Short read of status update (%d bytes)", len);
//       UVC_EXIT_VOID();
//       return;
//     }
//     UVC_DEBUG("Button (intf %u) %s len %d\n", data[1], data[3] ? "pressed" : "released", len);
    
//     if(devh->button_cb) {
//       UVC_DEBUG("Running user-supplied button callback");
//       devh->button_cb(data[1],
//                       data[3],
//                       devh->button_user_ptr);
//     }
//   } else {
//     UVC_DEBUG("Stream %u error event %02x %02x len %d.\n", data[1], data[2], data[3], len);
//   }

//   UVC_EXIT_VOID();
// }



//----------------------------------------------------------



// void uvc_process_status_xfer(uvc_device_handle_t *devh, struct libusb_transfer *transfer) {
  
  
//   printf("UVCLIB: START: uvc_process_status_xfer()\n");
  
//   UVC_ENTER();

//   /* printf("Got transfer of aLen = %d\n", transfer->actual_length); */

//   if (transfer->actual_length > 0) {
//     switch (transfer->buffer[0] & 0x0f) {
//     case 1: /* VideoControl interface */
//       printf("UVCLIB: uvc_process_status_xfer(): uvc_process_control_status()\n");
//       uvc_process_control_status(devh, transfer->buffer, transfer->actual_length);
//       break;
//     case 2:  /* VideoStreaming interface */
//       printf("UVCLIB: uvc_process_status_xfer(): uvc_process_streaming_status()\n");
//       uvc_process_streaming_status(devh, transfer->buffer, transfer->actual_length);
//       break;
//     }
//   }

//   printf("UVCLIB: END: uvc_process_status_xfer()\n");

//   UVC_EXIT_VOID();
// }




//----------------------------------------------------------



/** @internal
 * @brief Process asynchronous status updates from the device.
 */
void LIBUSB_CALL _uvc_status_callback_my(struct libusb_transfer *transfer) {

  printf("UVCLIB: _uvc_status_callback_my() ABABABABABABABABAB \n");

  // uvc_device_handle_t *devh = (uvc_device_handle_t *) transfer->user_data;

  switch (transfer->status) {
  case LIBUSB_TRANSFER_ERROR:
    printf("UVCLIB: CALLBACK: _uvc_status_callback() LIBUSB_TRANSFER_ERROR;\n");
  case LIBUSB_TRANSFER_CANCELLED:
    printf("UVCLIB: CALLBACK: _uvc_status_callback() LIBUSB_TRANSFER_CANCELLED;\n");
  case LIBUSB_TRANSFER_NO_DEVICE:
    printf("UVCLIB: CALLBACK: _uvc_status_callback() LIBUSB_TRANSFER_NO_DEVICE;\n");
    // UVC_EXIT_VOID();
    return;
  case LIBUSB_TRANSFER_COMPLETED:
     printf("UVCLIB: CALLBACK: _uvc_status_callback() LIBUSB_TRANSFER_COMPLETED; uvc_process_status_xfer()\n");
    //uvc_process_status_xfer(devh, transfer);
    break;
  case LIBUSB_TRANSFER_TIMED_OUT:
    printf("UVCLIB: CALLBACK: _uvc_status_callback() LIBUSB_TRANSFER_TIMED_OUT;\n");
  case LIBUSB_TRANSFER_STALL:
    printf("UVCLIB: CALLBACK: _uvc_status_callback() LIBUSB_TRANSFER_STALL;\n");
  case LIBUSB_TRANSFER_OVERFLOW:
    printf("UVCLIB: CALLBACK: _uvc_status_callback() LIBUSB_TRANSFER_OVERFLOW;\n");
    // UVC_DEBUG("retrying transfer, status = %d", transfer->status);
    break;
  }

  printf("UVCLIB: CALLBACK: _uvc_status_callback() libusb_submit_transfer();\n");
  libusb_submit_transfer(transfer);

}




//===============================================================
//===============================================================




/** @internal
 * @brief Event handler thread
 * There's one of these per UVC context.
 * @todo We shouldn't run this if we don't own the USB context
 */
void *_uvc_handle_events_my(void *arg) {

  printf("UVCLIB: START _uvc_handle_events_my() CDCDCDCDCDCDCDCDCDCD \n");

  struct libusb_context *usb_ctx = (libusb_context *) arg;

  while (!kill_handler_thread){
    printf("UVCLIB: libusb_handle_events_completed(): ctx->kill_handler_threadD %d\n", kill_handler_thread);
    //libusb_handle_events_completed(ctx->usb_ctx, &ctx->kill_handler_thread);
    libusb_handle_events_completed(usb_ctx, &kill_handler_thread);
  }

  printf("UVCLIB: END _uvc_handle_events_my()\n");  
  return NULL;
}







//===============================================================
//===============================================================
//===============================================================
//===============================================================







/** @internal
 * @brief Stream transfer callback
 *
 * Processes stream, places frames into buffer, signals listeners
 * (such as user callback thread and any polling thread) on new frame
 *
 * @param transfer Active transfer
 */

// This also works !!!
// void LIBUSB_CALL _uvc_stream_callback(struct libusb_transfer *transfer) {
//   printf("UVCLIB: _uvc_stream_callback BZBZBZBZBZBZBZ\n");
//   libusb_submit_transfer(transfer);
// }



void LIBUSB_CALL _uvc_stream_callback(struct libusb_transfer *transfer) {

  printf("UVCLIB: _uvc_stream_callback BZBZBZBZBZBZBZ\n");

  uvc_stream_handle_t *strmh = transfer->user_data;
  int resubmit = 1;

  switch (transfer->status) {
  case LIBUSB_TRANSFER_COMPLETED:
    printf("UVCLIB: CALLBACK _uvc_stream_callback 1: LIBUSB_TRANSFER_COMPLETED: transfer->num_iso_packets: %d\n", transfer->num_iso_packets);

    if (transfer->num_iso_packets == 0) {
      printf("UVCLIB: CALLBACK _uvc_stream_callback 2: This is a BULK mode transfer\n");
      _uvc_process_payload(strmh, transfer->buffer, transfer->actual_length);
    } else {
    
      printf("UVCLIB: CALLBACK _uvc_stream_callback 3: This is a ISO mode transfer\n");
      int packet_id;

      for (packet_id = 0; packet_id < transfer->num_iso_packets; ++packet_id) {
        printf("UVCLIB: CALLBACK _uvc_stream_callback 4: packet_id: %d\n", packet_id);

        uint8_t *pktbuf;
        struct libusb_iso_packet_descriptor *pkt;

        pkt = transfer->iso_packet_desc + packet_id;

        if (pkt->status != 0) {
          printf("UVCLIB: CALLBACK _uvc_stream_callback 5: bad packet (isochronous transfer); status: %d", pkt->status);
          continue;
        }

        printf("UVCLIB: CALLBACK _uvc_stream_callback 6: libusb_get_iso_packet_buffer_simple()\n");
        pktbuf = libusb_get_iso_packet_buffer_simple(transfer, packet_id);

        _uvc_process_payload(strmh, pktbuf, pkt->actual_length);      

      }

    }

    break;

  case LIBUSB_TRANSFER_CANCELLED:
    printf("UVCLIB: CALLBACK _uvc_stream_callback 6: LIBUSB_TRANSFER_CANCELLED\n");
  case LIBUSB_TRANSFER_ERROR:
    printf("UVCLIB: CALLBACK _uvc_stream_callback 7: LIBUSB_TRANSFER_ERROR\n");

  case LIBUSB_TRANSFER_NO_DEVICE:{
    printf("UVCLIB: CALLBACK _uvc_stream_callback 8: LIBUSB_TRANSFER_NO_DEVICE\n");
    int i;

    // !!! This logic is also required to stop the stream correctly.

    pthread_mutex_lock(&strmh->cb_mutex);

    /* Mark transfer as deleted. */
    for(i=0; i < LIBUVC_NUM_TRANSFER_BUFS; i++) {
      printf("UVCLIB: LIBUSB_TRANSFER_NO_DEVICE: i: %d\n", i);
      if(strmh->transfers[i] == transfer) {
        printf("UVCLIB: LIBUSB_TRANSFER_NO_DEVICE: Freeing transfer %d (%p) \n", i, transfer);
        free(transfer->buffer);
        libusb_free_transfer(transfer);
        strmh->transfers[i] = NULL;
        break;
      }
    }

    if(i == LIBUVC_NUM_TRANSFER_BUFS ) {
      printf("transfer %p not found; not freeing!", transfer);
    } 

    resubmit = 0;   

    pthread_cond_broadcast(&strmh->cb_cond);
    pthread_mutex_unlock(&strmh->cb_mutex);    
    break;
  }

  
  case LIBUSB_TRANSFER_TIMED_OUT:
    printf("UVCLIB: CALLBACK _uvc_stream_callback 9: LIBUSB_TRANSFER_TIMED_OUT\n");
  case LIBUSB_TRANSFER_STALL:
    printf("UVCLIB: CALLBACK _uvc_stream_callback 10: LIBUSB_TRANSFER_STALL\n");
  case LIBUSB_TRANSFER_OVERFLOW:
    printf("UVCLIB: CALLBACK _uvc_stream_callback 11: LIBUSB_TRANSFER_OVERFLOW\n");
    break; 

  }

  //---------------------------------------------

  if (resubmit) {

    printf("UVCLIB: CALLBACK _uvc_stream_callback 12: resubmit\n");

    if ( strmh->running ) {
      printf("UVCLIB: CALLBACK _uvc_stream_callback 13: resubmit\n");
      int libusbRet = libusb_submit_transfer(transfer);

      // !!! The rest of the logic is required for the "/* Wait for transfers to complete/cancel */"

      if (libusbRet < 0)
      {
        int i;
        pthread_mutex_lock(&strmh->cb_mutex);

        /* Mark transfer as deleted. */
        for (i = 0; i < LIBUVC_NUM_TRANSFER_BUFS; i++) {
          if (strmh->transfers[i] == transfer) {
            printf("UVCLIB: CALLBACK _uvc_stream_callback 14: Freeing transfer %d (%p)\n", i, transfer);
            free(transfer->buffer);
            libusb_free_transfer(transfer);
            strmh->transfers[i] = NULL;
            break;
          }
        }
        if (i == LIBUVC_NUM_TRANSFER_BUFS) {
          printf("UVCLIB: ERROR: CALLBACK _uvc_stream_callback 15: failed transfer %p not found; not freeing!\n", transfer);
        }

        pthread_cond_broadcast(&strmh->cb_cond);
        pthread_mutex_unlock(&strmh->cb_mutex);
      }
    } else {
      printf("UVCLIB: CALLBACK _uvc_stream_callback 16: resubmit\n");
      int i;
      pthread_mutex_lock(&strmh->cb_mutex);

      /* Mark transfer as deleted. */
      for(i=0; i < LIBUVC_NUM_TRANSFER_BUFS; i++) {
        if(strmh->transfers[i] == transfer) {
          printf("UVCLIB: CALLBACK _uvc_stream_callback 17: Freeing orphan transfer %d (%p)", i, transfer);
          free(transfer->buffer);
          libusb_free_transfer(transfer);
          strmh->transfers[i] = NULL;
          break;
        }
      }
      if(i == LIBUVC_NUM_TRANSFER_BUFS ) {
        printf("UVCLIB: ERROR: CALLBACK _uvc_stream_callback 18: orphan transfer %p not found; not freeing!", transfer);
      }

      pthread_cond_broadcast(&strmh->cb_cond);
      pthread_mutex_unlock(&strmh->cb_mutex);
    }

  }

}


//================================================================
//================================================================



//================================================================
//================================================================


/** @internal
 * @brief Populate the fields of a frame to be handed to user code
 * must be called with stream cb lock held!
 */
void _uvc_populate_frame(uvc_stream_handle_t *strmh) {


  uvc_frame_t *frame = &strmh->frame;

  // This is done in the uvc_parse_vs_frame_frame().
      // DL_APPEND(format->frame_descs, frame);

  //uvc_frame_t *frame = calloc(1, sizeof(*frame));

  frame->sequence = strmh->hold_seq;
  frame->capture_time_finished = strmh->capture_time_finished;

  printf("UVCLIB: _uvc_populate_frame: strmh->hold_bytes: %d\n", strmh->hold_bytes);


  /* copy the image data from the hold buffer to the frame (unnecessary extra buf?) */
  if (frame->data_bytes < strmh->hold_bytes) {
    frame->data = realloc(frame->data, strmh->hold_bytes);
  }
  frame->data_bytes = strmh->hold_bytes;
  memcpy(frame->data, strmh->holdbuf, frame->data_bytes);  

  if (strmh->meta_hold_bytes > 0)
  {
      if (frame->metadata_bytes < strmh->meta_hold_bytes)
      {
          frame->metadata = realloc(frame->metadata, strmh->meta_hold_bytes);
      }
      frame->metadata_bytes = strmh->meta_hold_bytes;
      memcpy(frame->metadata, strmh->meta_holdbuf, frame->metadata_bytes);
  }

}


//===========================================================

int fdnum_yuv;

/* This callback function runs once per frame. Use it to perform any
 * quick processing you need, or have it put the frame into your application's
 * input queue. If this function takes too long, you'll start losing frames. */
void cb(uvc_frame_t *frame) {

  // printf("cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb cb\n");

  printf("UVCLIB: cb(): frame->data_bytes: %d\n", frame->data_bytes);

  write(fdnum_yuv, frame->data, frame->data_bytes);
  //write(1, frame->data, frame->data_bytes);

}


//===========================================================



/** @internal
 * @brief User callback runner thread
 * @note There should be at most one of these per currently streaming device
 * @param arg Device handle
 */
void *_uvc_user_caller(void *arg) {

  printf("UVCLIB: _uvc_user_caller(): GHGHGHGHGHGHGHGHGHGHGHGHGHGHGH\n");

  uvc_stream_handle_t *strmh = (uvc_stream_handle_t *) arg;

  uint32_t last_seq = 0;

  do {
    printf("UVCLIB: _uvc_user_caller(): 1\n");
    pthread_mutex_lock(&strmh->cb_mutex);
    printf("UVCLIB: _uvc_user_caller(): 2\n");

    while (strmh->running && last_seq == strmh->hold_seq) {
      printf("UVCLIB: _uvc_user_caller(): 3\n");
      pthread_cond_wait(&strmh->cb_cond, &strmh->cb_mutex);
      printf("UVCLIB: _uvc_user_caller(): 4\n");
    }

    if (!strmh->running) {
      printf("UVCLIB: _uvc_user_caller(): 5\n");
      pthread_mutex_unlock(&strmh->cb_mutex);
      break;
    }

    printf("UVCLIB: _uvc_user_caller(): 5\n");
    
    last_seq = strmh->hold_seq;
    _uvc_populate_frame(strmh);

    printf("UVCLIB: _uvc_user_caller(): 7\n");
    
    pthread_mutex_unlock(&strmh->cb_mutex);

    printf("UVCLIB: _uvc_user_caller(): 8\n");
    
    cb(&strmh->frame);
    //strmh->user_cb(&strmh->frame, strmh->user_ptr);

  } while(1);

  return NULL; // return value ignored

}



//================================================================
//================================================================


long vidi;
long pidi;
char* tmpptr;

static struct libusb_device_handle *devh = NULL;
static struct libusb_device *usb_dev = NULL;


int main(int argc, char **argv){


  vidi = strtol(argv[1],&tmpptr,10);
  pidi = strtol(argv[2],&tmpptr,10);

  printf("UVCLIB: vidi: 0x%x\n", vidi);
  printf("UVCLIB: pidi: 0x%x\n", pidi); 


  bInterfaceNumber_contol_mysetting = strtol(argv[3],&tmpptr,10);
  bEndpointAddress_interrupt_mysetting = strtol(argv[4],&tmpptr,10);
  bcdUVC_mysetting = strtol(argv[5],&tmpptr,10);
  bInterfaceNumber_streaming_mysetting = strtol(argv[6],&tmpptr,10);
  bmHint_mysetting = strtol(argv[7],&tmpptr,10);
  bFormatIndex_mysetting = strtol(argv[8],&tmpptr,10);
  bFrameIndex_mysetting = strtol(argv[9],&tmpptr,10);
  dwFrameInterval_mysetting = strtol(argv[10],&tmpptr,10);
  wKeyFrameRate_mysetting = strtol(argv[11],&tmpptr,10);
  wPFrameRate_mysetting = strtol(argv[12],&tmpptr,10);
  wCompQuality_mysetting = strtol(argv[13],&tmpptr,10);
  wCompWindowSize_mysetting = strtol(argv[14],&tmpptr,10);
  wDelay_mysetting = strtol(argv[15],&tmpptr,10);
  dwMaxVideoFrameSize_mysetting = strtol(argv[16],&tmpptr,10);
  dwMaxPayloadTransferSize_mysetting = strtol(argv[17],&tmpptr,10);
  endpoint_bytes_per_packet_mysetting = strtol(argv[18],&tmpptr,10);
  bAlternateSetting_mysetting = strtol(argv[19],&tmpptr,10);
  bEndpointAddress_iso_mysetting = strtol(argv[20],&tmpptr,10);
  total_transfer_size_mysetting = strtol(argv[21],&tmpptr,10);


  //fmt_index = strtol(argv[3],tmpptr,10);
  //frame_index = strtol(argv[4],tmpptr,10);

  int ret;

  //================================================================
  //================================================================

  FILE* h_yuv = fopen("./out.yuv", "a+");
  fdnum_yuv = fileno(h_yuv);

  //================================================================



  uvc_stream_handle_t *strmh = NULL;
  strmh = calloc(1, sizeof(*strmh));

  //================================================================
  //================================================================

  struct libusb_context *ctx;

  ret = libusb_init(&ctx);


  if (ret != UVC_SUCCESS) {
    printf("EXAMPLE: ERROR: libusb_init() \n");
    return -1;
  }


  //================================================================
  //================================================================
  // https://github.com/nasa/astrobee/blob/30b91e3f821b02f5d850dbb962f4b4a8237b3913/hardware/vive/src/vive_driver/vive_usb.c

  // Get a list of devices
  libusb_device** devs;
  struct libusb_device_handle *devh = NULL;
  struct libusb_device_descriptor desc;
  struct libusb_device *dev;  


  ssize_t cnt = libusb_get_device_list(ctx, &devs);
  if (cnt < 0)
  {
    printf("UVCLIB: ERROR: the libusb_get_device_list function returned %d which is not zero \n", ret);
    return 0;
  }


  printf("UVCLIB: Devices cnt: %d \n", cnt);

  // Iterate over the devices looking for vive products
  int dev_found = 0;
	for (size_t idx = 0; idx < cnt; ++idx)
	{

    dev = devs[idx];

    libusb_get_device_descriptor(dev, &desc);

    printf("UVCLIB: desc.idVendor: 0x%x\n", desc.idVendor);
    printf("UVCLIB: desc.idProduct: 0x%x\n", desc.idProduct);

    if (desc.idVendor == vidi && desc.idProduct == pidi) {
      dev_found = 1;
      break;
    }
  }

  printf("UVCLIB: vidi: 0x%x\n", vidi);
  printf("UVCLIB: pidi: 0x%x\n", pidi);  

  if (!dev_found){
    printf("UVCLIB: ERROR: Device NOT Found\n");
    return -1;
  } else {
    printf("Device found\n");
  }


  printf("==============================================\n");
  printf("==============================================\n");
  printf("AFTER DEVICE FOUND (NOTHING IMPORTANT HAPPENED SO FAR)\n");
  printf("==============================================\n");
  printf("==============================================\n");


  //================================================================
  //================================================================


  printf("UVCLIB: START uvc_open()\n");
  struct libusb_device_handle *usb_devh;

  printf("UVCLIB: BEFORE libusb_open()\n");

  ret = libusb_open(dev, &usb_devh);

  printf("UVCLIB: AFTER libusb_open()\n");

  if (ret != UVC_SUCCESS) {
    printf("EXAMPLE: ERROR: libusb_open \n");
  }else{
    printf("EXAMPLE: SUCCESS: libusb_open \n");
  }   

  //================================================================
  //================================================================

  printf("UVCLIB: START uvc_open_internal()\n");

  printf("UVCLIB: START uvc_claim_if(): \n");

      uint8_t bInterfaceNumber_contol = bInterfaceNumber_contol_mysetting;

      ret = libusb_detach_kernel_driver(usb_devh, bInterfaceNumber_contol);

      if (ret == UVC_SUCCESS || ret == LIBUSB_ERROR_NOT_FOUND || ret == LIBUSB_ERROR_NOT_SUPPORTED) {
        printf("UVCLIB: claiming contol interface: %d\n", bInterfaceNumber_contol);

        ret = libusb_claim_interface(usb_devh, bInterfaceNumber_contol);

        if (ret < 0) {
            printf("Error claiming Control interface: %s\n", libusb_error_name(ret));
            //goto out;
        }else{
            printf("Control Interface Claimed\n");
        }
      }

  printf("UVCLIB: END uvc_claim_if(): \n");    

  printf("UVCLIB: uvc_open_internal() 4 \n");

  //================================================================
  //================================================================ 


  libusb_get_device_descriptor(dev, &desc);

  printf("UVCLIB: desc.idVendor: 0x%x\n", desc.idVendor);
  printf("UVCLIB: desc.idProduct: 0x%x\n", desc.idProduct);


  //================================================================
  //================================================================  

  printf("UVCLIB: uvc_open_internal() 5: libusb_alloc_transfer() \n");

  struct libusb_transfer *status_xfer = NULL;
  uint8_t status_buf[32];
  uint8_t bEndpointAddress_interrupt = bEndpointAddress_interrupt_mysetting;

  status_xfer = libusb_alloc_transfer(0);

  if (!status_xfer){
    printf("UVCLIB: ERROR: libusb_alloc_transfer(0)\n");
    return -1;
  }

  printf("UVCLIB: uvc_open_internal() 6: libusb_fill_interrupt_transfer(): bEndpointAddress_interrupt: %d \n", bEndpointAddress_interrupt);


  libusb_fill_interrupt_transfer(status_xfer,
                                usb_devh,
                                bEndpointAddress_interrupt,
                                status_buf,
                                sizeof(status_buf),
                                _uvc_status_callback_my,
                                devh,
                                0);

  printf("UVCLIB: uvc_open_internal() 7: libusb_submit_transfer() \n");

  ret = libusb_submit_transfer(status_xfer);

  printf("UVCLIB: uvc_open_internal() 8 \n");

  if (ret) {
    printf("UVCLIB: ERROR: uvc: device has a status interrupt endpoint, but unable to read from it\n");
    return -1;
  }

  //================================================================
  //================================================================  

  printf("UVCLIB: START uvc_start_handler_thread()\n");

  pthread_create(&handler_thread, NULL, _uvc_handle_events_my, (void*) ctx);

  printf("UVCLIB: END uvc_start_handler_thread()\n"); 

  //================================================================
  //================================================================  

  printf("UVCLIB: uvc_open_internal() 9 \n");
  printf("Device opened\n");


  printf("==============================================\n");
  printf("==============================================\n");
  printf("AFTER DEVICE OPENED \n");
  printf("==============================================\n");
  printf("==============================================\n");


  printf("UVCLIB: SLEEP(1) BEFORE START STREAMING SETTING UP\n");
  sleep(1);


  //================================================================
  //================================================================  


  printf("==============================================\n");
  printf("==============================================\n");
  printf("GET STREAM CTRL FORMAT SIZE \n");
  printf("==============================================\n");
  printf("==============================================\n");


  uint16_t bcdUVC = bcdUVC_mysetting;
  uint8_t probe = 0;
  enum uvc_req_code req = UVC_SET_CUR;


  strmh->cur_ctrl.bInterfaceNumber = bInterfaceNumber_streaming_mysetting;
  strmh->cur_ctrl.bmHint = bmHint_mysetting;
  strmh->cur_ctrl.bFormatIndex = bFormatIndex_mysetting;
  strmh->cur_ctrl.bFrameIndex = bFrameIndex_mysetting;
  strmh->cur_ctrl.dwFrameInterval = dwFrameInterval_mysetting;
  strmh->cur_ctrl.wKeyFrameRate = wKeyFrameRate_mysetting;
  strmh->cur_ctrl.wPFrameRate = wPFrameRate_mysetting;
  strmh->cur_ctrl.wCompQuality = wCompQuality_mysetting;
  strmh->cur_ctrl.wCompWindowSize = wCompWindowSize_mysetting;
  strmh->cur_ctrl.wDelay = wDelay_mysetting;
  strmh->cur_ctrl.dwMaxVideoFrameSize = dwMaxVideoFrameSize_mysetting;
  strmh->cur_ctrl.dwMaxPayloadTransferSize = dwMaxPayloadTransferSize_mysetting;


  if (bcdUVC >= 0x0110){
    // len = 34;
    // strmh->cur_ctrl.dwClockFrequency;
    // strmh->cur_ctrl.bmFramingInfo;
    // strmh->cur_ctrl.bPreferredVersion;
    // strmh->cur_ctrl.bMinVersion;
    // strmh->cur_ctrl.bMaxVersion;
  } else {
    //len = 26;
  }


  //================================================================
  //================================================================  

  printf("==============================================\n");
  printf("==============================================\n");
  printf("UVCLIB: START: uvc_stream_open_ctrl()\n");
  printf("==============================================\n");
  printf("==============================================\n");  

      printf("UVCLIB: START uvc_claim_if(): \n");

          ret = libusb_detach_kernel_driver(usb_devh, strmh->cur_ctrl.bInterfaceNumber);

          if (ret == UVC_SUCCESS || ret == LIBUSB_ERROR_NOT_FOUND || ret == LIBUSB_ERROR_NOT_SUPPORTED) {

            printf("UVCLIB: claiming streaming interface: %d\n", strmh->cur_ctrl.bInterfaceNumber);
            ret = libusb_claim_interface(usb_devh, strmh->cur_ctrl.bInterfaceNumber);

            if (ret < 0) {
                printf("Error claiming Control interface: %s\n", libusb_error_name(ret));
                //goto out;
            }else{
                printf("Control Interface Claimed\n");
            }    

          }

      printf("UVCLIB: END uvc_claim_if(): \n");


      printf("UVCLIB: START uvc_stream_ctrl()\n");
          printf("UVCLIB: START: uvc_query_stream_ctrl()\n");    

          uint8_t stream_buf[34];
          memset(stream_buf, 0, sizeof(stream_buf));

          size_t len;
          if (bcdUVC >= 0x0110)
            len = 34;
          else
            len = 26; 


          //================================================================
          /* prepare for a SET transfer */

          SHORT_TO_SW(strmh->cur_ctrl.bmHint, stream_buf);
          stream_buf[2] = strmh->cur_ctrl.bFormatIndex;
          stream_buf[3] = strmh->cur_ctrl.bFrameIndex; 
          INT_TO_DW(strmh->cur_ctrl.dwFrameInterval, stream_buf + 4);
          SHORT_TO_SW(strmh->cur_ctrl.wKeyFrameRate, stream_buf + 8);
          SHORT_TO_SW(strmh->cur_ctrl.wPFrameRate, stream_buf + 10);
          SHORT_TO_SW(strmh->cur_ctrl.wCompQuality, stream_buf + 12);
          SHORT_TO_SW(strmh->cur_ctrl.wCompWindowSize, stream_buf + 14);
          SHORT_TO_SW(strmh->cur_ctrl.wDelay, stream_buf + 16);
          INT_TO_DW(strmh->cur_ctrl.dwMaxVideoFrameSize, stream_buf + 18);
          INT_TO_DW(strmh->cur_ctrl.dwMaxPayloadTransferSize, stream_buf + 22);



          if (len == 34) {
            // INT_TO_DW ( strmh->cur_ctrl.dwClockFrequency, stream_buf + 26 );
            // stream_buf[30] = strmh->cur_ctrl.bmFramingInfo;
            // stream_buf[31] = strmh->cur_ctrl.bPreferredVersion;
            // stream_buf[32] = strmh->cur_ctrl.bMinVersion;
            // stream_buf[33] = strmh->cur_ctrl.bMaxVersion;
            // /** @todo support UVC 1.1 */                             // !!!!!!!!!!!!!!1
          }else{
          }


          /* do the transfer */
          ret = libusb_control_transfer(
              usb_devh,
              req == UVC_SET_CUR ? 0x21 : 0xA1,
              req,
              probe ? (UVC_VS_PROBE_CONTROL << 8) : (UVC_VS_COMMIT_CONTROL << 8),
              strmh->cur_ctrl.bInterfaceNumber,
              stream_buf, len, 0
          );
          //================================================================
                      
          printf("UVCLIB: END: uvc_query_stream_ctrl()\n");    
      printf("UVCLIB: END uvc_stream_ctrl()\n");

      strmh->running = 0;

      strmh->outbuf = malloc( strmh->cur_ctrl.dwMaxVideoFrameSize );
      strmh->holdbuf = malloc( strmh->cur_ctrl.dwMaxVideoFrameSize );

      strmh->meta_outbuf = malloc( LIBUVC_XFER_META_BUF_SIZE );
      strmh->meta_holdbuf = malloc( LIBUVC_XFER_META_BUF_SIZE );

      pthread_mutex_init(&strmh->cb_mutex, NULL);
      pthread_cond_init(&strmh->cb_cond, NULL);

  //================================================================
  //================================================================  


  printf("==============================================\n");
  printf("==============================================\n");
  printf("UVCLIB: uvc_stream_start()\n");
  printf("==============================================\n");
  printf("==============================================\n");  


  printf("UVCLIB: START: uvc_stream_start()\n");


  const struct libusb_interface *interface;
  int interface_id;

  strmh->running = 1;
  strmh->seq = 1;
  strmh->fid = 0;
  strmh->pts = 0;
  strmh->last_scr = 0;

  /* A VS interface uses isochronous transfers iff it has multiple altsettings.
   * (UVC 1.5: 2.4.3. VideoStreaming Interface) */
  char isochronous = 1;



  /* For isochronous streaming, we choose an appropriate altsetting for the endpoint
    * and set up several transfers */
  const struct libusb_interface_descriptor *altsetting = 0;
  const struct libusb_endpoint_descriptor *endpoint = 0; 

  /* Number of packets per transfer */
  size_t packets_per_transfer = 32;
  /* Size of packet transferable from the chosen endpoint */
  size_t endpoint_bytes_per_packet = endpoint_bytes_per_packet_mysetting;



  uint8_t bAlternateSetting = bAlternateSetting_mysetting;
  uint8_t bEndpointAddress_iso = bEndpointAddress_iso_mysetting;
  size_t total_transfer_size = total_transfer_size_mysetting;
  

  struct libusb_transfer *transfer;


    printf("UVCLIB: libusb_set_interface_alt_setting(): strmh->cur_ctrl.bInterfaceNumber(bInterfaceNumber_streaming_mysetting ???) %d\n", strmh->cur_ctrl.bInterfaceNumber);
    printf("UVCLIB: libusb_set_interface_alt_setting(): bAlternateSetting(bAlternateSetting_mysetting) %d\n", bAlternateSetting);
    printf("UVCLIB: libusb_set_interface_alt_setting(): bEndpointAddress_iso(bEndpointAddress_iso_mysetting) %d\n", bEndpointAddress_iso);




  ret = libusb_set_interface_alt_setting(usb_devh, strmh->cur_ctrl.bInterfaceNumber, bAlternateSetting);

  if (ret != UVC_SUCCESS) {
    printf("libusb_set_interface_alt_setting failed\n");
    strmh->running = 0;
    return -1;
  }

  printf("=============================================\n");
  printf("=============================================\n");
  printf("UVCLIB: END: uvc_stream_start()\n");
  printf("SLEEP 1(1) Before 'libusb_alloc_transfer', 'libusb_fill_iso_transfer' \n");
  sleep(1);
  printf("=============================================\n");
  printf("=============================================\n");


  



  for (int transfer_id = 0; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; ++transfer_id) {

    printf("libusb_alloc_transfer(): %d: packets_per_transfer %d\n", transfer_id, packets_per_transfer);
    transfer = libusb_alloc_transfer(packets_per_transfer);

    strmh->transfers[transfer_id] = transfer;
    strmh->transfer_bufs[transfer_id] = malloc(total_transfer_size);

    // Helper function to populate the required libusb_transfer fields for an isochronous transfer.
    // _uvc_stream_callback: callback function to be invoked on transfer completion
    // 500: timeout for the transfer in milliseconds
    libusb_fill_iso_transfer(
      transfer, usb_devh, bEndpointAddress_iso,
      strmh->transfer_bufs[transfer_id],
      total_transfer_size, packets_per_transfer, _uvc_stream_callback, (void*) strmh, 5000);

    libusb_set_iso_packet_lengths(transfer, endpoint_bytes_per_packet);  

  }


  printf("=============================================\n");
  printf("=============================================\n");
  printf("AFTER 'libusb_alloc_transfer', 'libusb_fill_iso_transfer' \n");
  printf("SLEEP 2(1) Before 'libusb_submit_transfer'\n");
  sleep(1);
  printf("=============================================\n");
  printf("=============================================\n");  


  pthread_t cb_thread;
  pthread_create(&cb_thread, NULL, _uvc_user_caller, (void*) strmh);  



  int transfer_id;
  for (transfer_id = 0; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; transfer_id++) {
    printf("UVCLIB: libusb_submit_transfer(): %d\n", transfer_id);
    ret = libusb_submit_transfer(strmh->transfers[transfer_id]);
    if (ret != UVC_SUCCESS) {
      printf("UVCLIB: ERROR: libusb_submit_transfer failed 1: %d",ret);
      break;
    }
  }


  if ( ret != UVC_SUCCESS && transfer_id >= 0 ) {
    printf("UVCLIB: ERROR: libusb_submit_transfer failed 2\n");
    for ( ; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; transfer_id++) 
    {
      printf("!!!dgnet: UVCLIB: FREE TRANSFER: %d\n", transfer_id);
      free ( strmh->transfers[transfer_id]->buffer );
      libusb_free_transfer ( strmh->transfers[transfer_id]);
      strmh->transfers[transfer_id] = 0;
    }
    ret = UVC_SUCCESS;
  }  


  printf("=============================================\n");
  printf("=============================================\n");
  printf("AFTER 'libusb_submit_transfer'\n");
  printf("SLEEP 3 (10) STREAMING...\n");
  sleep(10);   
  printf("=============================================\n");
  printf("=============================================\n");


  // if ( ret != UVC_SUCCESS && transfer_id >= 0 ) {
  //   for ( ; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; transfer_id++) {
  //     free (transfers[transfer_id]->buffer );
  //     libusb_free_transfer (transfers[transfer_id]);
  //     transfers[transfer_id] = 0;
  //   }
  // }



  //================================================================
  //================================================================ 
  //================================================================
  //================================================================    



  printf("UVCLIB: START uvc_stop_streaming()\n");

  //uvc_stop_streaming(devh);`
      // DL_FOREACH_SAFE(devh->streams, strmh, strmh_tmp) {
      //   uvc_stream_close(strmh);

              if (strmh->running){
                printf("UVCLIB: uvc_stop_streaming() 1: LIBUVC_NUM_TRANSFER_BUFS: %d\n", LIBUVC_NUM_TRANSFER_BUFS);
                //uvc_stream_stop(strmh);
                      strmh->running = 0;
                      pthread_mutex_lock(&strmh->cb_mutex);
                      int i;
                      for(i=0; i < LIBUVC_NUM_TRANSFER_BUFS; i++) {
                        printf("UVCLIB: uvc_stop_streaming() 2, i: %d\n", i);
                        if(strmh->transfers[i] != NULL){
                          printf("UVCLIB: uvc_stop_streaming() 3, i: %d\n", i);
                          libusb_cancel_transfer(strmh->transfers[i]);
                        }
                      }
                      // printf("======================================\n");
                      // printf("======================================\n");     
                      // printf("UVCLIB: SLEEP(3) BEFORE LOOP\n");            
                      // printf("======================================\n");
                      // printf("======================================\n");

                      // sleep(3);

                      /* Wait for transfers to complete/cancel */
                      do {
                        printf("UVCLIB: uvc_stop_streaming() 4\n");
                        for(i=0; i < LIBUVC_NUM_TRANSFER_BUFS; i++) {
                          printf("UVCLIB: uvc_stop_streaming() 5, i: %d\n", i);
                          if(strmh->transfers[i] != NULL){
                            printf("UVCLIB: uvc_stop_streaming() 6, i: %d\n", i);
                            break;
                          }
                        }
                        if(i == LIBUVC_NUM_TRANSFER_BUFS ){
                          printf("break z: i: %d, LIBUVC_NUM_TRANSFER_BUFS: %d\n", i, LIBUVC_NUM_TRANSFER_BUFS); 
                          break;
                        }

                        printf("UVCLIB: uvc_stop_streaming() 7 z\n"); 

                        // !!! This will not work without logic in the "if ( resubmit )"
                        pthread_cond_wait(&strmh->cb_cond, &strmh->cb_mutex);
                        //sleep(0.2);
                      } while(1);

                      // Kick the user thread awake
                      pthread_cond_broadcast(&strmh->cb_cond);
                      pthread_mutex_unlock(&strmh->cb_mutex);


                      printf("UVCLIB: uvc_stop_streaming() 8\n");

                      /** @todo stop the actual stream, camera side? */

                      // if (strmh->user_cb) {
                      //   /* wait for the thread to stop (triggered by
                      //   * LIBUSB_TRANSFER_CANCELLED transfer) */
                      pthread_join(cb_thread, NULL);
                      // }
              }

                      // return UVC_SUCCESS;



              // uvc_release_if(strmh->devh, strmh->stream_if->bInterfaceNumber);

                    printf("UVCLIB: START: uvc_release_if()\n");
                      printf("UVCLIB: uvc_release_if() 1: libusb_set_interface_alt_setting()\n");
                      // libusb_set_interface_alt_setting(devh->usb_devh, idx, 0);
                      libusb_set_interface_alt_setting(usb_devh, strmh->cur_ctrl.bInterfaceNumber, 0);
                      printf("UVCLIB: uvc_release_if() 2\n");
                      // ret = libusb_release_interface(devh->usb_devh, idx);
                      ret = libusb_release_interface(usb_devh, strmh->cur_ctrl.bInterfaceNumber);
                      // ret = libusb_attach_kernel_driver(devh->usb_devh, idx);
                      ret = libusb_attach_kernel_driver(usb_devh, strmh->cur_ctrl.bInterfaceNumber);
                    printf("UVCLIB: END: uvc_release_if() \n");

              pthread_cond_destroy(&strmh->cb_cond);
              pthread_mutex_destroy(&strmh->cb_mutex);


              //DL_DELETE(strmh->devh->streams, strmh);
              //free(strmh);


  //---------------------------------------------------
  printf("UVCLIB: END uvc_stop_streaming()\n");
  //---------------------------------------------------



  //---------------------------------------------------
  printf("UVCLIB: START uvc_close()\n");
  //---------------------------------------------------

  // void uvc_close(uvc_device_handle_t *devh) {
      // if (devh->streams)
            // uvc_stop_streaming(devh);

      // uvc_release_if(devh, devh->info->ctrl_if.bInterfaceNumber);      

          printf("UVCLIB: START: uvc_release_if()\n");
            printf("UVCLIB: uvc_release_if() 1: libusb_set_interface_alt_setting()\n");
            // libusb_set_interface_alt_setting(devh->usb_devh, idx, 0);
            libusb_set_interface_alt_setting(usb_devh, bInterfaceNumber_contol, 0);
            printf("UVCLIB: uvc_release_if() 2\n");
            // ret = libusb_release_interface(devh->usb_devh, idx);
            ret = libusb_release_interface(usb_devh, bInterfaceNumber_contol);
            // ret = libusb_attach_kernel_driver(devh->usb_devh, idx);
            ret = libusb_attach_kernel_driver(usb_devh, bInterfaceNumber_contol);
          printf("UVCLIB: END: uvc_release_if() \n");


  printf("UVCLIB: uvc_close() 3: libusb_close(devh->usb_devh); pthread_join();\n");
  //if (ctx->own_usb_ctx && ctx->open_devices == devh && devh->next == NULL) {
      kill_handler_thread = 1;
      libusb_close(usb_devh);
      pthread_join(handler_thread, NULL);

      // uvc_unref_device(devh->dev);
      // uvc_free_devh(devh);

    // DL_DELETE(ctx->open_devices, devh);
    // uvc_unref_device(devh->dev);
    // uvc_free_devh(devh);


  //---------------------------------------------------
  printf("UVCLIB: END uvc_close()\n");
  //---------------------------------------------------


  // uvc_unref_device(dev);


  //---------------------------------------------------
  printf("UVCLIB: START uvc_exit()\n");
  //---------------------------------------------------

  // void uvc_exit(uvc_context_t *ctx) {

      // DL_FOREACH(ctx->open_devices, devh) {
      //   uvc_close(devh);
      // }    

      // if (ctx->own_usb_ctx)
      //   printf("UVCLIB: uvc_exit() 2 libusb_exit()\n");
           libusb_exit(ctx);
           //free(ctx);



  //---------------------------------------------------
  printf("UVCLIB: STOP uvc_exit()\n");
  //---------------------------------------------------

  //================================================================
  //================================================================   


  return 0;


  // https://github.com/JulienBernard3383279/plain-adapter-logger/blob/80cca2daf027aa95ea549048d52c9bbcd668d236/PlainAdapterLogger.cpp

}