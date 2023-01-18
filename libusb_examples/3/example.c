
// sudo ./example 1133 2085     // c270
// sudo ./example 10549 6        // megawell

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>  // for strtol
#include <errno.h>


#include <pthread.h>
#include <signal.h>




void LIBUSB_CALL _uvc_stream_callback(struct libusb_transfer *transfer) {

  printf("_uvc_stream_callback BZBZBZBZBZBZBZ \n");

}


void *_uvc_user_caller(void *arg) {

  do {    
    //printf("strmh->user_cb(&strmh->frame, strmh->user_ptr);\n");
    sleep(1);
  } while(1);


}




void LIBUSB_CALL _uvc_status_callback(struct libusb_transfer *transfer) {

    printf("_uvc_status_callback BZBZBZBZBZBZBZ\n");
    libusb_submit_transfer(transfer);

}



void *_uvc_handle_events(void *arg) {

  printf("LIBUVC: _uvc_handle_events 1\n");

  int kill_handler_thread = 1;

  while (kill_handler_thread){
    sleep(1);
    //printf("LIBUVC: libusb_handle_events_completed\n");
    libusb_handle_events_completed(NULL, &kill_handler_thread);
  }

  return NULL;
}








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




int fdnum_yuv;
int fdnum_bgr;


long vidi;
long pidi;
char* tmpptr;


unsigned int width_p;
unsigned int height_p;
int fmt_index;
int frame_index;


static struct libusb_device_handle *dev_handle = NULL;
static struct libusb_device *usb_dev = NULL;


/** Streaming mode, includes all information needed to select stream
 * @ingroup streaming
 */
typedef struct uvc_stream_ctrl {
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
  uint8_t bInterfaceNumber;
} uvc_stream_ctrl_t;



/** UVC request code (A.8) */
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



int main(int argc, char **argv){


  vidi = strtol(argv[1],tmpptr,10);
  pidi = strtol(argv[2],tmpptr,10);

  //fmt_index = strtol(argv[3],tmpptr,10);
  //frame_index = strtol(argv[4],tmpptr,10);


  //================================================================
  //================================================================


  // LIBUVC: ctrl->bmHint: 1
  // LIBUVC: ctrl->bFormatIndex: 1
  // LIBUVC: ctrl->bFrameIndex: 1
  // LIBUVC: ctrl->dwFrameInterval: 333333
  // LIBUVC: ctrl->wKeyFrameRate: 0
  // LIBUVC: ctrl->wPFrameRate: 0
  // LIBUVC: ctrl->wCompQuality: 2000
  // LIBUVC: ctrl->wCompWindowSize: 0
  // LIBUVC: ctrl->wDelay: 0
  // LIBUVC: ctrl->dwMaxVideoFrameSize: 614400
  // LIBUVC: ctrl->dwMaxPayloadTransferSize: 3060
  // LIBUVC: ctrl->bInterfaceNumber: 1


  uint8_t buf[34];
  memset(buf, 0, sizeof(buf));

  uvc_error_t err;

  size_t len;
  uint16_t bcdUVC;
  uint8_t probe;
  enum uvc_req_code req;

  uvc_stream_ctrl_t ctrl;


  probe = 0;

  bcdUVC = 0x100;
  if (bcdUVC >= 0x0110)
    len = 34;
  else
    len = 26;


  req = UVC_SET_CUR;
  /* prepare for a SET transfer */
  if (req == UVC_SET_CUR) {

    ctrl.bInterfaceNumber = 1;

    ctrl.bmHint = 1;
    ctrl.bFormatIndex = 1;
    ctrl.bFrameIndex = 18;
    ctrl.dwFrameInterval = 2000000;
    ctrl.wKeyFrameRate = 0;
    ctrl.wPFrameRate = 0;
    ctrl.wCompQuality = 2000;
    ctrl.wCompWindowSize = 0;
    ctrl.wDelay = 0;
    ctrl.dwMaxVideoFrameSize = 1843200;
    ctrl.dwMaxPayloadTransferSize = 3060;
    

    SHORT_TO_SW(ctrl.bmHint, buf);
    buf[2] = ctrl.bFormatIndex;
    buf[3] = ctrl.bFrameIndex;
    INT_TO_DW(ctrl.dwFrameInterval, buf + 4);
    SHORT_TO_SW(ctrl.wKeyFrameRate, buf + 8);
    SHORT_TO_SW(ctrl.wPFrameRate, buf + 10);
    SHORT_TO_SW(ctrl.wCompQuality, buf + 12);
    SHORT_TO_SW(ctrl.wCompWindowSize, buf + 14);
    SHORT_TO_SW(ctrl.wDelay, buf + 16);
    INT_TO_DW(ctrl.dwMaxVideoFrameSize, buf + 18);
    INT_TO_DW(ctrl.dwMaxPayloadTransferSize, buf + 22);

    if(len=34){

      //ctrl.dwClockFrequency = ;
      //ctrl.bmFramingInfo = ;
      //ctrl.bPreferredVersion = ;
      //ctrl.bMinVersion = ;
      //ctrl.bMaxVersion = ;

      // INT_TO_DW ( ctrl.dwClockFrequency, buf + 26 );
      // buf[30] = ctrl.bmFramingInfo;
      // buf[31] = ctrl.bPreferredVersion;
      // buf[32] = ctrl.bMinVersion;
      // buf[33] = ctrl.bMaxVersion;

    }
  
  }


  char isochronous;

  isochronous = 1;



  //================================================================
  //================================================================



  int rc;
  rc = libusb_init(NULL);
  if (rc < 0) {
      fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(rc));
      exit(1);
  }

  dev_handle = libusb_open_device_with_vid_pid(NULL, vidi, pidi);

  if (!dev_handle) {
      fprintf(stderr, "Error finding USB device\n");
      return 1;
  }

  usb_dev = libusb_get_device(dev_handle) ;


  //================================================================
  //================================================================







  uint8_t control_bEndpointAddress = 135;
  uint8_t control_bInterfaceNumber = 0;

  rc = libusb_detach_kernel_driver(dev_handle, control_bInterfaceNumber);
  if (rc == UVC_SUCCESS || rc == LIBUSB_ERROR_NOT_FOUND || rc== LIBUSB_ERROR_NOT_SUPPORTED) {
      printf("claiming Control interface %d\n", control_bInterfaceNumber);
  }


  // claim Control interface
  rc = libusb_claim_interface(dev_handle, control_bInterfaceNumber);  


  if (rc < 0) {
      printf("Error claiming Control interface: %s\n", libusb_error_name(rc));
      //goto out;
  }else{
      printf("Control Interface Claimed\n");
  }


  struct libusb_transfer *status_xfer = NULL;
  uint8_t status_buf[32];


  status_xfer = libusb_alloc_transfer(0);

  if (!status_xfer) {
    printf("Error allocate Control transfer");
    //goto out;
  }

  int cnud = 123;

  libusb_fill_interrupt_transfer(status_xfer,
                                  dev_handle,
                                  control_bInterfaceNumber,
                                  status_buf,
                                  sizeof(status_buf),
                                  _uvc_status_callback,
                                  (void*) cnud,
                                  0);

  rc = libusb_submit_transfer(status_xfer);

  if (rc){
    printf("device has a status interrupt endpoint, but unable to read from it\n");
    //goto out;
  }




  //================================================================
  //================================================================


  pthread_t handler_thread;
  pthread_create(&handler_thread, NULL, _uvc_handle_events, (void*) cnud);


  libusb_exit(NULL);


  //================================================================
  //================================================================



//   rc = libusb_detach_kernel_driver(dev_handle, ctrl.bInterfaceNumber);
//   if (rc == UVC_SUCCESS || rc == LIBUSB_ERROR_NOT_FOUND || rc== LIBUSB_ERROR_NOT_SUPPORTED) {
//       printf("claiming interface %d\n", ctrl.bInterfaceNumber);
//   }

//   // claim streaming interface
//   rc = libusb_claim_interface(dev_handle, ctrl.bInterfaceNumber);
//   if (rc < 0) {
//       printf("Error claiming Stream interface: %s\n", libusb_error_name(rc));
//       goto out;
//   }else{
//       printf("Stream Interface Claimed\n");
//   }



//   //sleep(2);
//   //================================================================
//   //================================================================


//   printf("-----START libusb_control_transfer, synchronous-----\n");

//   /* do the transfer */
//   err = libusb_control_transfer(
//       dev_handle,
//       req == UVC_SET_CUR ? 0x21 : 0xA1,
//       req,
//       probe ? (UVC_VS_PROBE_CONTROL << 8) : (UVC_VS_COMMIT_CONTROL << 8),
//       ctrl.bInterfaceNumber,
//       buf, len, 0
//   );


//   printf("-----END libusb_control_transfer, synchronous-----\n");

//   //================================================================
//   //================================================================
//   //sleep(2);

//   printf("-----START STREAMING... uvc_stream_start()-----\n");

//   if (isochronous) {

//     /* For isochronous streaming, we choose an appropriate altsetting for the endpoint
//      * and set up several transfers */
//     const struct libusb_interface_descriptor *altsetting = 0;
//     const struct libusb_endpoint_descriptor *endpoint = 0; 

//     /* Number of packets per transfer */
//     size_t packets_per_transfer = 32;
//     /* Size of packet transferable from the chosen endpoint */
//     size_t endpoint_bytes_per_packet = 3060;


//     uint8_t bInterfaceNumber = ctrl.bInterfaceNumber;
//     uint8_t bAlternateSetting = 11;
//     uint8_t bEndpointAddres = 129;
//     size_t total_transfer_size = 97920;

//     int LIBUVC_NUM_TRANSFER_BUFS = 100;

//     struct libusb_transfer *transfer;
//     struct libusb_transfer *transfers[LIBUVC_NUM_TRANSFER_BUFS];
//     uint8_t *transfer_bufs[LIBUVC_NUM_TRANSFER_BUFS];


//     rc = libusb_set_interface_alt_setting(dev_handle, bInterfaceNumber, bAlternateSetting);

//     if (rc != UVC_SUCCESS) {
//       printf("libusb_set_interface_alt_setting failed\n");
//       goto out;
//     }



//     int ud = 123;

//     for (int transfer_id = 0; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; ++transfer_id) {

//       printf("libusb_alloc_transfer(): %d: packets_per_transfer %d\n", transfer_id, packets_per_transfer);
//       transfer = libusb_alloc_transfer(packets_per_transfer);

//       transfers[transfer_id] = transfer;
//       transfer_bufs[transfer_id] = malloc(total_transfer_size);

//       libusb_fill_iso_transfer(
//         transfer, dev_handle, bEndpointAddres,
//         transfer_bufs[transfer_id],
//         total_transfer_size, packets_per_transfer, _uvc_stream_callback, &ud, 5000);


//       libusb_set_iso_packet_lengths(transfer, endpoint_bytes_per_packet);  

//     }

//     int cbud = 123;
//     pthread_t cb_thread;
//     pthread_create(&cb_thread, NULL, _uvc_user_caller, (void*) cbud);





//     for (int transfer_id = 0; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; transfer_id++) {
//       printf("libusb_submit_transfer(): %d\n", transfer_id);
//       rc = libusb_submit_transfer(transfers[transfer_id]);
//       if (rc != UVC_SUCCESS) {
//         printf("libusb_submit_transfer failed: %d",rc);
//         break;
//       }
//     }


//     for (int transfer_id ; transfer_id < LIBUVC_NUM_TRANSFER_BUFS; transfer_id++) {
//       free (transfers[transfer_id]->buffer );
//       libusb_free_transfer (transfers[transfer_id]);
//       transfers[transfer_id] = 0;
//     }

//   }

//   sleep(10);

//   printf("-----END STREAMING...-----\n");

//   //================================================================
//   //================================================================


//   /* libusb_release_interface *should* reset the alternate setting to the first available,
//       but sometimes (e.g. on Darwin) it doesn't. Thus, we do it explicitly here.
//       This is needed to de-initialize certain cameras. */
//   libusb_set_interface_alt_setting(dev_handle, ctrl.bInterfaceNumber, 0);
//   rc = libusb_release_interface(dev_handle, ctrl.bInterfaceNumber);


//   if (rc == UVC_SUCCESS) {
//       printf("reattached kernel driver to interface %d\n", ctrl.bInterfaceNumber);
//   } else if (rc == LIBUSB_ERROR_NOT_FOUND || rc == LIBUSB_ERROR_NOT_SUPPORTED) {
//       rc = UVC_SUCCESS;  /* NOT_FOUND and NOT_SUPPORTED are OK: nothing to do */
//   } else {
//       printf("error reattaching kernel driver to interface %d: error: %d", ctrl.bInterfaceNumber, rc);
//   }



//   goto out;
// out:
//   printf("OUT\n");
//   if (dev_handle)
//       libusb_close(dev_handle);
//   libusb_exit(NULL);
//   return rc;


}