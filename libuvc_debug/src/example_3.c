/*
 * Created by alexB, 2/18/2108
 */
 
#include "libuvc/libuvc.h"
#include <stdio.h>
#include <unistd.h>

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
  res = uvc_find_device(ctx, &dev,
      0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
  if (res < 0) {
    uvc_perror(res, "uvc_find_device"); /* no devices found */
  } else {
    fprintf(stderr, "Device found\n");
    /* Try to open the device: requires exclusive access */
    res = uvc_open(dev, &devh);
    if (res < 0) {
      uvc_perror(res, "uvc_open"); /* unable to open device */
    } else {
      fprintf(stderr, "Device opened\n");
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
          sleep(10);
          /* End the stream. Blocks until last callback is serviced */
          uvc_stop_streaming(devh);
          sleep(3); /* sleep 3 sec */
          fprintf(stderr, "Done streaming\n");
        }
      }
      /* Release our handle on the device */
      uvc_close(devh);
      fprintf(stderr, "Device closed\n");
      fclose(1);
    }
    /* Release the device descriptor */
    uvc_unref_device(dev);
  }
  /* Close the UVC context. This closes and cleans up any existing device handles,
   * and it closes the libusb context if one was not provided. */
  fprintf(stderr, "Device unrefed\n");
  uvc_exit(ctx);
  fprintf(stderr, "UVC exited");
  return 0;
}