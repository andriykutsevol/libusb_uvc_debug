/*
 * Created by alexB, 2/18/2108
 */
 
#include "libuvc/libuvc.h"
#include "libuvc/libuvc_internal.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libusb.h>

//=============================================================
//=============================================================
//=============================================================


//  error: invalid use of incomplete typedef
//  Pointer to Incomplete type "struct" not allowed
// https://stackoverflow.com/questions/15069107/pointer-to-incomplete-class-type-not-allowed

// This error is given when you are trying to use a struct that has been forward-declared, 
// but NOT DEFINED.
// While it is absolutely OK to declare and manipulate pointers to such structs
// rying to dereference them is not OK
// because the compiler needs to know their size and layout in order to perform the access.

// You need to include a definition of the struct node
// in the compilation unit where you define your "some" function to fix this problem.

// struct uvc_device {
//   struct uvc_context *ctx;
//   int ref;
//   libusb_device *usb_dev;
// };




// struct uvc_device_handle {
//   struct uvc_device *dev;
//   struct uvc_device_handle *prev, *next;
//   /** Underlying USB device handle */
//   libusb_device_handle *usb_devh;
//   struct uvc_device_info *info;
//   struct libusb_transfer *status_xfer;
//   uint8_t status_buf[32];
//   /** Function to call when we receive status updates from the camera */
//   uvc_status_callback_t *status_cb;
//   void *status_user_ptr;
//   /** Function to call when we receive button events from the camera */
//   uvc_button_callback_t *button_cb;
//   void *button_user_ptr;

//   uvc_stream_handle_t *streams;
//   /** Whether the camera is an iSight that sends one header per frame */
//   uint8_t is_isight;
//   uint32_t claimed;
// };


//=============================================================
//=============================================================
//=============================================================


static uvc_error_t get_device_descriptor(
        uvc_device_handle_t *devh,
        uvc_device_descriptor_t **desc) {
  uvc_device_descriptor_t *desc_internal;
  struct libusb_device_descriptor usb_desc;
  struct libusb_device_handle *usb_devh = devh->usb_devh;
  uvc_error_t ret;

  UVC_ENTER();

  ret = libusb_get_device_descriptor(devh->dev->usb_dev, &usb_desc);

  if (ret != UVC_SUCCESS) {
    UVC_EXIT(ret);
    return ret;
  }

  desc_internal = calloc(1, sizeof(*desc_internal));
  desc_internal->idVendor = usb_desc.idVendor;
  desc_internal->idProduct = usb_desc.idProduct;

  unsigned char buf[64];

  int bytes = libusb_get_string_descriptor_ascii(
          usb_devh, usb_desc.iSerialNumber, buf, sizeof(buf));

  if (bytes > 0)
    desc_internal->serialNumber = strdup((const char*) buf);

  bytes = libusb_get_string_descriptor_ascii(
          usb_devh, usb_desc.iManufacturer, buf, sizeof(buf));

  if (bytes > 0)
    desc_internal->manufacturer = strdup((const char*) buf);

  bytes = libusb_get_string_descriptor_ascii(
          usb_devh, usb_desc.iProduct, buf, sizeof(buf));

  if (bytes > 0)
    desc_internal->product = strdup((const char*) buf);

  *desc = desc_internal;

  UVC_EXIT(ret);
  return ret;
}



//=============================================================
//=============================================================
//=============================================================




/** @internal
 * @brief Process asynchronous status updates from the device.
 */
void LIBUSB_CALL _uvc_status_callback_my(struct libusb_transfer *transfer) {

  printf("!!!dgnet: UVCLIB: _uvc_status_callback BZBZBZBZ \n");

  UVC_ENTER();

  libusb_submit_transfer(transfer);

  UVC_EXIT_VOID();
}





/** @internal
 * @brief Event handler thread
 * There's one of these per UVC context.
 * @todo We shouldn't run this if we don't own the USB context
 */
void *_uvc_handle_events_my(void *arg) {

  printf("!!!dgnet: UVCLIB: START _uvc_handle_events()\n");

  uvc_context_t *ctx = (uvc_context_t *) arg;

  while (!ctx->kill_handler_thread){
    printf("!!!dgnet: UVCLIB: END libusb_handle_events_completed()\n");
    libusb_handle_events_completed(ctx->usb_ctx, &ctx->kill_handler_thread);
  }

  printf("!!!dgnet: UVCLIB: END _uvc_handle_events()\n");  
  return NULL;
}





//=============================================================
//=============================================================
//=============================================================


/* This callback function runs once per frame. Use it to perform any
 * quick processing you need, or have it put the frame into your application's
 * input queue. If this function takes too long, you'll start losing frames. 
 */
void cb(uvc_frame_t *frame, void *ptr) {
  uvc_frame_t *bgr;
  uvc_error_t ret;
  write(1, frame->data, frame->data_bytes);
}

int main(int argc, char **argv) {
  uvc_context_t *ctx;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;
  uvc_stream_ctrl_t ctrl;
  uvc_error_t res;
  int counter = 0;
  /* Initialize a UVC service context. Libuvc will set up its own libusb
   * context. Replace NULL with a libusb_context pointer to run libuvc
   * from an existing libusb context.   */
  res = uvc_init(&ctx, NULL);
  if (res < 0) {
    uvc_perror(res, "uvc_init");
    return res;
  }
  fprintf(stderr, "UVC initialized\n");
  /* Locates the first attached UVC device, stores in dev */
  res = uvc_find_device(ctx, &dev, 0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
  if (res < 0) {
    uvc_perror(res, "uvc_find_device"); /* no devices found */
  } else {
    fprintf(stderr, "Device found\n");

    printf("==============================================\n");
    printf("==============================================\n");
    printf("AFTER DEVICE FOUND (NOTHING IMPORTANT HAPPENED SO FAR)\n");
    printf("==============================================\n");
    printf("==============================================\n");


    //--------------------------------------------------------    
    //--------------------------------------------------------

    printf("!!!dgnet: UVCLIB: START uvc_open()\n");
    // res = uvc_open(dev, &devh);
    // uvc_error_t uvc_open(uvc_device_t *dev, uvc_device_handle_t **devh) {

        uvc_error_t ret;
        struct libusb_device_handle *usb_devh;

        printf("!!!dgnet: UVCLIB: BEFORE libusb_open()\n");

        ret = libusb_open(dev->usb_dev, &usb_devh);

        printf("!!!dgnet: UVCLIB: AFTER libusb_open()\n");

        if (ret != UVC_SUCCESS) {
          printf("EXAMPLE: ERROR: libusb_open \n");
        }else{
          printf("EXAMPLE: SUCCESS: libusb_open \n");
        }    


        //--------------------------------------------------------
        //--------------------------------------------------------

        printf("!!!dgnet: UVCLIB: START uvc_open_internal()\n");
        // ret = uvc_open_internal(dev, usb_devh, devh);
        // static uvc_error_t uvc_open_internal(uvc_device_t *dev, struct libusb_device_handle *usb_devh, uvc_device_handle_t **devh) {


            uvc_device_handle_t *internal_devh;
            struct libusb_device_descriptor desc;

            uvc_ref_device(dev);


            internal_devh = calloc(1, sizeof(*internal_devh));
            internal_devh->dev = dev;

            internal_devh->usb_devh = usb_devh;

            printf("!!!dgnet: UVCLIB: uvc_open_internal() 1: uvc_get_device_info() \n");


            //--------------------------------------------------------
            //--------------------------------------------------------

            printf("!!!dgnet: UVCLIB: START uvc_get_device_info()\n");
            //ret = uvc_get_device_info(internal_devh, &(internal_devh->info));
            // uvc_error_t uvc_get_device_info(uvc_device_handle_t *devh, uvc_device_info_t **info) {

                UVC_ENTER();

                uvc_device_info_t *internal_info;

                internal_info = calloc(1, sizeof(*internal_info));
                if (!internal_info) {
                  UVC_EXIT(UVC_ERROR_NO_MEM);
                  printf("!!!dgnet: UVCLIB: ERROR: calloc\n");
                  return UVC_ERROR_NO_MEM;
                }  

                printf("!!!dgnet: UVCLIB: uvc_get_device_info() 1: libusb_get_config_descriptor() \n");          

                if (libusb_get_config_descriptor(internal_devh->dev->usb_dev, 0, &(internal_info->config)) != 0) {
                  free(internal_info);
                  UVC_EXIT(UVC_ERROR_IO);
                  printf("!!!dgnet: UVCLIB: ERROR: libusb_get_config_descriptor()\n");
                  return UVC_ERROR_IO;
                }

                printf("!!!dgnet: UVCLIB: uvc_get_device_info() 2\n");


                //--------------------------------------------------------
                //--------------------------------------------------------



                printf("!!!dgnet: UVCLIB: START uvc_scan_control()\n");
                //ret = uvc_scan_control(internal_devh, internal_info);
                // uvc_error_t uvc_scan_control(uvc_device_handle_t *devh, uvc_device_info_t *info) {                

                    const struct libusb_interface_descriptor *if_desc;
                    uvc_error_t parse_ret;
                    int interface_idx;
                    const unsigned char *buffer;
                    size_t buffer_left, block_size;

                    UVC_ENTER();
                    ret = UVC_SUCCESS;
                    if_desc = NULL;

                    uvc_device_descriptor_t* dev_desc;
                    int haveTISCamera = 0;

                    printf("!!!dgnet: UVCLIB: uvc_scan_control() 1: get_device_descriptor()\n");

                    get_device_descriptor ( internal_devh, &dev_desc );
                    if ( 0x199e == dev_desc->idVendor && ( 0x8101 == dev_desc->idProduct ||
                        0x8102 == dev_desc->idProduct )) {
                      haveTISCamera = 1;
                    }

                    printf("!!!dgnet: UVCLIB: uvc_scan_control() 2\n");

                    uvc_free_device_descriptor ( dev_desc );

                    for (interface_idx = 0; interface_idx < internal_info->config->bNumInterfaces; ++interface_idx) {
                      if_desc = &internal_info->config->interface[interface_idx].altsetting[0];

                      if ( haveTISCamera && if_desc->bInterfaceClass == 255 && if_desc->bInterfaceSubClass == 1) // Video, Control
                        break;

                      if (if_desc->bInterfaceClass == 14 && if_desc->bInterfaceSubClass == 1) // Video, Control
                        break;

                      if_desc = NULL;
                    }


                    if (if_desc == NULL) {
                      printf("!!!dgnet: UVCLIB: ERROR: UVC_ERROR_INVALID_DEVICE \n");
                      UVC_EXIT(UVC_ERROR_INVALID_DEVICE);
                      return -1;
                    }


                    internal_info->ctrl_if.bInterfaceNumber = interface_idx;
                    if (if_desc->bNumEndpoints != 0) {
                      internal_info->ctrl_if.bEndpointAddress = if_desc->endpoint[0].bEndpointAddress;
                    } 

                    buffer = if_desc->extra;
                    buffer_left = if_desc->extra_length;

                    printf("!!!dgnet: UVCLIB: uvc_scan_control() 3\n");



                    while (buffer_left >= 3) { // parseX needs to see buf[0,2] = length,type
                      block_size = buffer[0];
                      parse_ret = uvc_parse_vc(internal_devh->dev, internal_info, buffer, block_size);

                      if (parse_ret != UVC_SUCCESS) {
                        ret = parse_ret;
                        break;
                      }

                      buffer_left -= block_size;
                      buffer += block_size;
                    }

                    UVC_EXIT(ret);                                                                


                printf("!!!dgnet: UVCLIB: END uvc_scan_control()\n");    

                //--------------------------------------------------------
                //--------------------------------------------------------


                if (ret != UVC_SUCCESS) {
                  uvc_free_device_info(internal_info);
                  printf("!!!dgnet: UVCLIB: ERROR: uvc_scan_control()\n");
                  UVC_EXIT(ret);
                  return ret;
                }

                internal_devh->info = internal_info;

                UVC_EXIT(ret);

            printf("!!!dgnet: UVCLIB: END uvc_get_device_info()\n");            


            //--------------------------------------------------------
            //--------------------------------------------------------

            printf("!!!dgnet: UVCLIB: uvc_open_internal() 2 \n");

            printf("!!!dgnet: UVCLIB: uvc_open_internal() 3: uvc_claim_if(): bInterfaceNumber: %d \n", internal_devh->info->ctrl_if.bInterfaceNumber);

            
            
            printf("!!!dgnet: UVCLIB: START uvc_claim_if(): \n");
            // ret = uvc_claim_if(internal_devh, internal_devh->info->ctrl_if.bInterfaceNumber);
            // uvc_error_t uvc_claim_if(uvc_device_handle_t *devh, int idx)

                UVC_ENTER();

                if ( internal_devh->claimed & ( 1 << internal_devh->info->ctrl_if.bInterfaceNumber )) {
                  printf("!!!dgnet: UVCLIB: ERROR: attempt to claim already-claimed interface %d\n", internal_devh->info->ctrl_if.bInterfaceNumber );
                  UVC_EXIT(ret);
                  return -1;
                }

                ret = libusb_detach_kernel_driver(internal_devh->usb_devh, internal_devh->info->ctrl_if.bInterfaceNumber);

                if (ret == UVC_SUCCESS || ret == LIBUSB_ERROR_NOT_FOUND || ret == LIBUSB_ERROR_NOT_SUPPORTED) {
                  UVC_DEBUG("claiming interface %d", internal_devh->info->ctrl_if.bInterfaceNumber);

                  printf("!!!dgnet: UVCLIB: uvc_claim_if(): idx: %d\n", internal_devh->info->ctrl_if.bInterfaceNumber);
                  if (!( ret = libusb_claim_interface(internal_devh->usb_devh, internal_devh->info->ctrl_if.bInterfaceNumber))) {
                    internal_devh->claimed |= ( 1 << internal_devh->info->ctrl_if.bInterfaceNumber );
                  }

                } else {
                  UVC_DEBUG("not claiming interface %d: unable to detach kernel driver (%s)",
                            internal_devh->info->ctrl_if.bInterfaceNumber, uvc_strerror(ret));
                }

                UVC_EXIT(ret);


            printf("!!!dgnet: UVCLIB: END uvc_claim_if(): \n");


            printf("!!!dgnet: UVCLIB: uvc_open_internal() 4 \n");            


            //--------------------------------------------------------
            //--------------------------------------------------------

            libusb_get_device_descriptor(dev->usb_dev, &desc);
            internal_devh->is_isight = (desc.idVendor == 0x05ac && desc.idProduct == 0x8501);


            //--------------------------------------------------------
            //--------------------------------------------------------

            printf("!!!dgnet: UVCLIB: uvc_open_internal() 5: libusb_alloc_transfer() \n");  



            if (internal_devh->info->ctrl_if.bEndpointAddress) {
              internal_devh->status_xfer = libusb_alloc_transfer(0);
              if (!internal_devh->status_xfer) {
                ret = UVC_ERROR_NO_MEM;
                printf("!!!dgnet: UVCLIB: ERROR: libusb_alloc_transfer(0)\n");
                return -1;
              }

              printf("!!!dgnet: UVCLIB: uvc_open_internal() 6: libusb_fill_interrupt_transfer():  bEndpointAddress: %d\n", internal_devh->info->ctrl_if.bEndpointAddress);

              libusb_fill_interrupt_transfer(internal_devh->status_xfer,
                                            usb_devh,
                                            internal_devh->info->ctrl_if.bEndpointAddress,
                                            internal_devh->status_buf,
                                            sizeof(internal_devh->status_buf),
                                            _uvc_status_callback_my,
                                            internal_devh,
                                            0);


              printf("!!!dgnet: UVCLIB: uvc_open_internal() 7: libusb_submit_transfer() \n");

              ret = libusb_submit_transfer(internal_devh->status_xfer);
              UVC_DEBUG("libusb_submit_transfer() = %d", ret);

              printf("!!!dgnet: UVCLIB: uvc_open_internal() 8 \n");

              if (ret) {
                printf("!!!dgnet: UVCLIB: ERROR: uvc: device has a status interrupt endpoint, but unable to read from it\n");
                return -1;
              }
            }

            //--------------------------------------------------------
            //--------------------------------------------------------

            if (dev->ctx->own_usb_ctx && dev->ctx->open_devices == NULL) {
              /* Since this is our first device, we need to spawn the event handler thread */
              
              printf("!!!dgnet: UVCLIB: START uvc_start_handler_thread()\n");
              //uvc_start_handler_thread(dev->ctx);
              // void uvc_start_handler_thread(uvc_context_t *ctx) {

              if (dev->ctx->own_usb_ctx)
                pthread_create(&ctx->handler_thread, NULL, _uvc_handle_events_my, (void*) ctx);

              printf("!!!dgnet: UVCLIB: END uvc_start_handler_thread()\n"); 


            }            

            //--------------------------------------------------------
            //--------------------------------------------------------

            printf("!!!dgnet: UVCLIB: uvc_open_internal() 9 \n");


            DL_APPEND(dev->ctx->open_devices, internal_devh);
            devh = internal_devh;

            UVC_EXIT(ret)

            //--------------------------------------------------------
            //--------------------------------------------------------



        printf("!!!dgnet: UVCLIB: END uvc_open_internal()\n");

    printf("!!!dgnet: UVCLIB: END uvc_open()\n");
















    if (res < 0) {
      uvc_perror(res, "uvc_open"); /* unable to open device */
    } else {
      printf("Device opened\n");



    printf("==============================================\n");
    printf("==============================================\n");
    printf("AFTER DEVICE OPENED \n");
    printf("==============================================\n");
    printf("==============================================\n");


      /* Print out a message containing all the information that libuvc
       * knows about the device */
      uvc_print_diag(devh, stderr);
      int fmt = UVC_FRAME_FORMAT_YUYV;
      /* Try to negotiate a 640x480 30 fps YUYV stream profile */
      res = uvc_get_stream_ctrl_format_size(
          devh, &ctrl, /* result stored in ctrl */
          fmt, /* YUV 422, aka YUV 4:2:2. try _COMPRESSED */
          640, 480, 30 /* width, height, fps */
      );
      if (res < 0) {
        uvc_perror(res, "get_mode"); /* device doesn't provide a matching stream */
      } else {
        /* Print out the result */
        fprintf(stderr, "uvc_print_stream_ctrl:\n");
        uvc_print_stream_ctrl(&ctrl, stderr);        
        /* Start the video stream. The library will call user function cb:
         *   cb(frame, (void*) 12345) */
        res = uvc_start_streaming(devh, &ctrl, cb, (void*)12345, 0);
        if (res < 0) {
          uvc_perror(res, "start_streaming"); /* unable to start stream */
        } else {
          fprintf(stderr, "Streaming...\n");
          uvc_set_ae_mode(devh, 1); /* e.g., turn on auto exposure */
          sleep(100);
          /* End the stream. Blocks until last callback is serviced */
          uvc_stop_streaming(devh);
          sleep(3); /* sleep 3 sec */
          fprintf(stderr, "Done streaming\n");
        }
      }


      /* Release our handle on the device */
      uvc_close(devh);
      fprintf(stderr, "Device closed\n");
      // fclose(1);


    }










    /* Release the device descriptor */
    uvc_unref_device(dev);
  }
  /* Close the UVC context. This closes and cleans up any existing device handles,
   * and it closes the libusb context if one was not provided. */
  fprintf(stderr, "Device unrefed\n");
  sleep(5);
  uvc_exit(ctx);
  fprintf(stderr, "UVC exited\n");
  return 0;
}