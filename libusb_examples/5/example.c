
// sudo ./example 1133 2085     // c270
// sudo ./example 10549 6        // megawell

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>  // for strtol
#include <errno.h>


#include <pthread.h>
#include <signal.h>


//================================================================
//================================================================

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

//================================================================
//================================================================

pthread_t handler_thread;
int kill_handler_thread = 0;


/** @internal
 * @brief Process asynchronous status updates from the device.
 */
void LIBUSB_CALL _uvc_status_callback_my(struct libusb_transfer *transfer) {

  printf("UVCLIB: _uvc_status_callback_my() ABABABABABABABABAB \n");

  libusb_submit_transfer(transfer);

}





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
    libusb_handle_events_completed(usb_ctx, kill_handler_thread);
  }

  printf("UVCLIB: END _uvc_handle_events_my()\n");  
  return NULL;
}


//===============================================================


/** @internal
 * @brief Stream transfer callback
 *
 * Processes stream, places frames into buffer, signals listeners
 * (such as user callback thread and any polling thread) on new frame
 *
 * @param transfer Active transfer
 */
void LIBUSB_CALL _uvc_stream_callback(struct libusb_transfer *transfer) {

  printf("UVCLIB: _uvc_stream_callback BZBZBZBZBZBZBZ\n");

}



/** @internal
 * @brief User callback runner thread
 * @note There should be at most one of these per currently streaming device
 * @param arg Device handle
 */
void *_uvc_user_caller(void *arg) {


  printf("UVCLIB: _uvc_user_caller(): GHGHGHGHGHGHGHGHGHGHGHGHGHGHGH\n");

}



//================================================================
//================================================================

long vidi;
long pidi;
char* tmpptr;

static struct libusb_device_handle *devh = NULL;
static struct libusb_device *usb_dev = NULL;


int main(int argc, char **argv){


  vidi = strtol(argv[1],tmpptr,10);
  pidi = strtol(argv[2],tmpptr,10);

  //fmt_index = strtol(argv[3],tmpptr,10);
  //frame_index = strtol(argv[4],tmpptr,10);

  int ret;

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

    //printf("desc.idVendor: 0x%x\n", desc.idVendor);
    //printf("desc.idProduct: 0x%x\n", desc.idProduct);

    if (desc.idVendor == vidi && desc.idProduct == pidi) {
      dev_found = 1;
      break;
    }
  }

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

      uint8_t contol_bInterfaceNumber = 0;

      ret = libusb_detach_kernel_driver(usb_devh, contol_bInterfaceNumber);

      if (ret == UVC_SUCCESS || ret == LIBUSB_ERROR_NOT_FOUND || ret == LIBUSB_ERROR_NOT_SUPPORTED) {
        printf("UVCLIB: claiming contol interface: %d\n", contol_bInterfaceNumber);

        ret = libusb_claim_interface(usb_devh, contol_bInterfaceNumber);

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
  uint8_t bEndpointAddress = 135;

  status_xfer = libusb_alloc_transfer(0);

  if (!status_xfer){
    printf("UVCLIB: ERROR: libusb_alloc_transfer(0)\n");
    return -1;
  }

  printf("UVCLIB: uvc_open_internal() 6: libusb_fill_interrupt_transfer() \n");


  libusb_fill_interrupt_transfer(status_xfer,
                                usb_devh,
                                bEndpointAddress,
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


  //================================================================
  //================================================================  


  printf("==============================================\n");
  printf("==============================================\n");
  printf("GET STREAM CTRL FORMAT SIZE \n");
  printf("==============================================\n");
  printf("==============================================\n");


  uint16_t bcdUVC = 0x100;
  uint8_t probe = 0;
  enum uvc_req_code req = UVC_SET_CUR;


  uint8_t ctrl_bInterfaceNumber = 1;
  uint16_t ctrl_bmHint = 1;
  uint8_t ctrl_bFormatIndex = 1;
  uint8_t ctrl_bFrameIndex = 1;
  uint32_t ctrl_dwFrameInterval = 333333;
  uint16_t ctrl_wKeyFrameRate = 0;
  uint16_t ctrl_wPFrameRate = 0;
  uint16_t ctrl_wCompQuality = 200;
  uint16_t ctrl_wCompWindowSize = 0;
  uint16_t ctrl_wDelay = 0;
  uint32_t ctrl_dwMaxVideoFrameSize = 614400;
  uint32_t ctrl_dwMaxPayloadTransferSize = 3060;

  if (bcdUVC >= 0x0110){
    // len = 34;
    // uint32_t dwClockFrequency;
    // uint8_t bmFramingInfo;
    // uint8_t bPreferredVersion;
    // uint8_t bMinVersion;
    // uint8_t bMaxVersion;
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

          ret = libusb_detach_kernel_driver(usb_devh, ctrl_bInterfaceNumber);

          if (ret == UVC_SUCCESS || ret == LIBUSB_ERROR_NOT_FOUND || ret == LIBUSB_ERROR_NOT_SUPPORTED) {
            printf("UVCLIB: claiming streaming interface: %d\n", ctrl_bInterfaceNumber);

            ret = libusb_claim_interface(usb_devh, ctrl_bInterfaceNumber);

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

          SHORT_TO_SW(ctrl_bmHint, stream_buf);
          stream_buf[2] = ctrl_bFormatIndex;
          stream_buf[3] = ctrl_bFrameIndex; 
          INT_TO_DW(ctrl_dwFrameInterval, stream_buf + 4);
          SHORT_TO_SW(ctrl_wKeyFrameRate, stream_buf + 8);
          SHORT_TO_SW(ctrl_wPFrameRate, stream_buf + 10);
          SHORT_TO_SW(ctrl_wCompQuality, stream_buf + 12);
          SHORT_TO_SW(ctrl_wCompWindowSize, stream_buf + 14);
          SHORT_TO_SW(ctrl_wDelay, stream_buf + 16);
          INT_TO_DW(ctrl_dwMaxVideoFrameSize, stream_buf + 18);
          INT_TO_DW(ctrl_dwMaxPayloadTransferSize, stream_buf + 22);



          if (len == 34) {
            // INT_TO_DW ( ctrl_dwClockFrequency, stream_buf + 26 );
            // stream_buf[30] = ctrl_bmFramingInfo;
            // stream_buf[31] = ctrl_bPreferredVersion;
            // stream_buf[32] = ctrl_bMinVersion;
            // stream_buf[33] = ctrl_bMaxVersion;
            // /** @todo support UVC 1.1 */                             // !!!!!!!!!!!!!!1
          }else{
          }


          /* do the transfer */
          ret = libusb_control_transfer(
              usb_devh,
              req == UVC_SET_CUR ? 0x21 : 0xA1,
              req,
              probe ? (UVC_VS_PROBE_CONTROL << 8) : (UVC_VS_COMMIT_CONTROL << 8),
              ctrl_bInterfaceNumber,
              stream_buf, len, 0
          );          

          //================================================================
                      
          printf("UVCLIB: END: uvc_query_stream_ctrl()\n");    
      printf("UVCLIB: END uvc_stream_ctrl()\n");


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
  size_t endpoint_bytes_per_packet = 3060;


  uint8_t bInterfaceNumber = ctrl_bInterfaceNumber;
  uint8_t bAlternateSetting = 11;
  uint8_t bEndpointAddres = 129;
  size_t total_transfer_size = 97920;
  int LIBUVC_NUM_TRANSFER_BUFS = 100;


  struct libusb_transfer *transfer;
  struct libusb_transfer *transfers[LIBUVC_NUM_TRANSFER_BUFS];
  uint8_t *transfer_bufs[LIBUVC_NUM_TRANSFER_BUFS];


  ret = libusb_set_interface_alt_setting(usb_devh, bInterfaceNumber, bAlternateSetting);

  if (ret != UVC_SUCCESS) {
    printf("libusb_set_interface_alt_setting failed\n");
    return -1;
  }

  printf("SLEEP(3)\n");
  sleep(3);

  printf("UVCLIB: END: uvc_stream_start()\n");



  for (int transfer_id = 0; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; ++transfer_id) {

    printf("libusb_alloc_transfer(): %d: packets_per_transfer %d\n", transfer_id, packets_per_transfer);
    transfer = libusb_alloc_transfer(packets_per_transfer);

    transfers[transfer_id] = transfer;
    transfer_bufs[transfer_id] = malloc(total_transfer_size);

    libusb_fill_iso_transfer(
      transfer, usb_devh, bEndpointAddres,
      transfer_bufs[transfer_id],
      total_transfer_size, packets_per_transfer, _uvc_stream_callback, 0, 5000);


    libusb_set_iso_packet_lengths(transfer, endpoint_bytes_per_packet);  

  }


  // pthread_t cb_thread;
  // pthread_create(&cb_thread, NULL, _uvc_user_caller, 0);  




    for (int transfer_id = 0; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; transfer_id++) {
      printf("libusb_submit_transfer(): %d\n", transfer_id);
      ret = libusb_submit_transfer(transfers[transfer_id]);
      if (ret != UVC_SUCCESS) {
        printf("libusb_submit_transfer failed: %d",ret);
        break;
      }
    }


    for (int transfer_id ; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; transfer_id++) {
      free (transfers[transfer_id]->buffer );
      libusb_free_transfer (transfers[transfer_id]);
      transfers[transfer_id] = 0;
    }



  //================================================================
  //================================================================ 
  //================================================================
  //================================================================   


  printf("==============================================\n");
  printf("==============================================\n");
  printf("UVCLIB: STREAMING........\n");
  sleep(5);
  printf("==============================================\n");
  printf("==============================================\n");  


  //================================================================
  //================================================================ 
  //================================================================
  //================================================================    



  printf("UVCLIB: START uvc_exit()\n");

  //uvc_stop_streaming(devh);`
      // DL_FOREACH_SAFE(devh->streams, strmh, strmh_tmp) {
      //   uvc_stream_close(strmh);

              // if (strmh->running)
              //   uvc_stream_stop(strmh);
                      // strmh->running = 0;
                      // pthread_mutex_lock(&strmh->cb_mutex);

                      // for(i=0; i < LIBUVC_NUM_TRANSFER_BUFS; i++) {
                      //   if(strmh->transfers[i] != NULL)
                      //     libusb_cancel_transfer(strmh->transfers[i]);
                      // }                      


                      // /* Wait for transfers to complete/cancel */
                      // do {
                      //   for(i=0; i < LIBUVC_NUM_TRANSFER_BUFS; i++) {
                      //     if(strmh->transfers[i] != NULL)
                      //       break;
                      //   }
                      //   if(i == LIBUVC_NUM_TRANSFER_BUFS )
                      //     break;
                      //   pthread_cond_wait(&strmh->cb_cond, &strmh->cb_mutex);
                      // } while(1);


                      // /** @todo stop the actual stream, camera side? */

                      // if (strmh->user_cb) {
                      //   /* wait for the thread to stop (triggered by
                      //   * LIBUSB_TRANSFER_CANCELLED transfer) */
                      //   pthread_join(strmh->cb_thread, NULL);
                      // }

                      // return UVC_SUCCESS;



              // uvc_release_if(strmh->devh, strmh->stream_if->bInterfaceNumber);

                    printf("UVCLIB: START: uvc_release_if()\n");
                      printf("UVCLIB: uvc_release_if() 1: libusb_set_interface_alt_setting()\n");
                      libusb_set_interface_alt_setting(usb_devh, contol_bInterfaceNumber, 0);
                      printf("UVCLIB: uvc_release_if() 2\n");
                      ret = libusb_release_interface(usb_devh, contol_bInterfaceNumber);
                      ret = libusb_attach_kernel_driver(usb_devh, contol_bInterfaceNumber);
                    printf("UVCLIB: END: uvc_release_if() \n");

              // pthread_cond_destroy(&strmh->cb_cond);
              // pthread_mutex_destroy(&strmh->cb_mutex);





  printf("UVCLIB: uvc_close() 3: libusb_close(devh->usb_devh); pthread_join();\n");
  kill_handler_thread = 1;
  libusb_close(usb_devh);
  pthread_join(handler_thread, NULL);
  //pthread_join(cb_thread, NULL);

  libusb_exit(ctx);
  //free(ctx);


  printf("UVCLIB: END uvc_exit()\n");

  //================================================================
  //================================================================   


  return 0;


  // https://github.com/JulienBernard3383279/plain-adapter-logger/blob/80cca2daf027aa95ea549048d52c9bbcd668d236/PlainAdapterLogger.cpp

}