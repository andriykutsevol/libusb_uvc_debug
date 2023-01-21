#include "libuvc/libuvc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>  // for strtol


int fdnum_yuv;
int fdnum_bgr;


long vidi;
long pidi;
char* tmpptr;


unsigned int width_p;
unsigned int height_p;
int fmt_index;
int frame_index;


/* This callback function runs once per frame. Use it to perform any
 * quick processing you need, or have it put the frame into your application's
 * input queue. If this function takes too long, you'll start losing frames. */
void cb(uvc_frame_t *frame, void *ptr) {

  uvc_frame_t *bgr;
  uvc_error_t ret;

  enum uvc_frame_format *frame_format = (enum uvc_frame_format *)ptr;
  /* FILE *fp;
   * static int jpeg_count = 0;
   * static const char *H264_FILE = "iOSDevLog.h264";
   * static const char *MJPEG_FILE = ".jpeg";
   * char filename[16]; */

  /* We'll convert the image from YUV/JPEG to BGR, so allocate space */
  bgr = uvc_allocate_frame(frame->width * frame->height * 3);
  if (!bgr) {
    printf("unable to allocate bgr frame!\n");
    return;
  }

  printf("callback! frame_format = %d, width = %d, height = %d, length = %lu, ptr = %d\n",
    frame->frame_format, frame->width, frame->height, frame->data_bytes, (int) ptr);

  switch (frame->frame_format) {
  case UVC_FRAME_FORMAT_H264:
    printf("1\n");
    /* use `ffplay H264_FILE` to play */
    /* fp = fopen(H264_FILE, "a");
     * fwrite(frame->data, 1, frame->data_bytes, fp);
     * fclose(fp); */
    break;
  case UVC_COLOR_FORMAT_MJPEG:
    printf("2\n");
    /* sprintf(filename, "%d%s", jpeg_count++, MJPEG_FILE);
     * fp = fopen(filename, "w");
     * fwrite(frame->data, 1, frame->data_bytes, fp);
     * fclose(fp); */
    break;
  case UVC_COLOR_FORMAT_YUYV:
    printf("3\n");
    /* Do the BGR conversion */
    // ret = uvc_any2bgr(frame, bgr);
    // if (ret) {
    //   uvc_perror(ret, "uvc_any2bgr");
    //   uvc_free_frame(bgr);
    //   return;
    // }
    write(fdnum_yuv, frame->data, frame->data_bytes);
    break;
  default:
    printf("4\n");
    break;
  }

  if (frame->sequence % 30 == 0) {
    printf(" * got image %u\n",  frame->sequence);
  }

  /* Call a user function:
   *
   * my_type *my_obj = (*my_type) ptr;
   * my_user_function(ptr, bgr);
   * my_other_function(ptr, bgr->data, bgr->width, bgr->height);
   */

  /* Call a C++ method:
   *
   * my_type *my_obj = (*my_type) ptr;
   * my_obj->my_func(bgr);
   */

  /* Use opencv.highgui to display the image:
   * 
   * cvImg = cvCreateImageHeader(
   *     cvSize(bgr->width, bgr->height),
   *     IPL_DEPTH_8U,
   *     3);
   *
   * cvSetData(cvImg, bgr->data, bgr->width * 3); 
   *
   * cvNamedWindow("Test", CV_WINDOW_AUTOSIZE);
   * cvShowImage("Test", cvImg);
   * cvWaitKey(10);
   *
   * cvReleaseImageHeader(&cvImg);
   */

  uvc_free_frame(bgr);
}

int main(int argc, char **argv) {
  uvc_context_t *ctx;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;
  uvc_stream_ctrl_t ctrl;
  uvc_error_t res;

  FILE* h_yuv = fopen("./out", "a+");
  fdnum_yuv = fileno(h_yuv);


  /* Initialize a UVC service context. Libuvc will set up its own libusb
   * context. Replace NULL with a libusb_context pointer to run libuvc
   * from an existing libusb context. */
  res = uvc_init(&ctx, NULL);

  if (res < 0) {
    uvc_perror(res, "uvc_init");
    return res;
  }

  puts("UVC initialized");


  vidi = strtol(argv[1],tmpptr,10);
  pidi = strtol(argv[2],tmpptr,10);

  fmt_index = strtol(argv[3],tmpptr,10);
  frame_index = strtol(argv[4],tmpptr,10);


  // // google
  // res = uvc_find_device(
  //     ctx, &dev,
  //     6353, 20510, NULL); /* filter devices: vendor_id, product_id, "serial_num" */

  // // aver
  // res = uvc_find_device(
  //     ctx, &dev,
  //     1994, 4371, NULL); /* filter devices: vendor_id, product_id, "serial_num" */

  // Default
  /* Locates the first attached UVC device, stores in dev */
    res = uvc_find_device(
      ctx, &dev,
      vidi, pidi, NULL); /* filter devices: vendor_id, product_id, "serial_num" */


  if (res < 0) {
    uvc_perror(res, "uvc_find_device"); /* no devices found */
  } else {
    puts("Device found");

    /* Try to open the device: requires exclusive access */
    res = uvc_open(dev, &devh);

    if (res < 0) {
      uvc_perror(res, "uvc_open"); /* unable to open device */
    } else {
      puts("Device opened");

      /* Print out a message containing all the information that libuvc
       * knows about the device */
      uvc_print_diag(devh, stderr);


      //-------------------------------

      const uvc_format_desc_t *format_desc = uvc_get_format_descs(devh);

      for (int i=0; i<fmt_index; i++ ){
        // Guvcview: "Camera Output: NV12, YUYV, RGB3, BGR3 ... etc"
        printf("EXAMPLE: FMT_INDEX: %d\n", i);
        format_desc = format_desc->next;
      }

      //-------------------------------

      const uvc_frame_desc_t *frame_desc = format_desc->frame_descs;


      for (int i=0; i<frame_index; i++){
        // Guvcview: "Rrsolution"
        printf("EXAMPLE: FRAME_INDEX: %d\n", i);
        frame_desc = frame_desc->next;
      }

      //-------------------------------

      enum uvc_frame_format frame_format;
      int width = 640;
      int height = 480;
      int fps = 30;

      switch (format_desc->bDescriptorSubtype) {
      case UVC_VS_FORMAT_MJPEG:
        frame_format = UVC_COLOR_FORMAT_MJPEG;
        break;
      case UVC_VS_FORMAT_FRAME_BASED:
        frame_format = UVC_FRAME_FORMAT_H264;
        break;
      default:
        frame_format = UVC_FRAME_FORMAT_YUYV;
        break;
      }

      if (frame_desc) {
        width = frame_desc->wWidth;
        height = frame_desc->wHeight;
        fps = 10000000 / frame_desc->dwDefaultFrameInterval;
      }

      printf("\nEXAMPLE: Format: (%4s) %dx%d %dfps\n", format_desc->fourccFormat, width, height, fps);

      printf("EXAMPLE: uvc_get_stream_ctrl_format_size(): frame_format: %d\n", frame_format);

      /* Try to negotiate first stream profile */
      res = uvc_get_stream_ctrl_format_size(
          devh, &ctrl, /* result stored in ctrl */
          frame_format,
          width, height, fps /* width, height, fps */
      );


      printf("\EXAMPLE: res %d\n", res);

      /* Print out the result */
      uvc_print_stream_ctrl(&ctrl, stderr);

      if (res < 0) {
        printf("EXAMPLE: ERROR: uvc_get_stream_ctrl_format_size()");
        uvc_perror(res, "get_mode"); /* device doesn't provide a matching stream */
      } else {
        /* Start the video stream. The library will call user function cb:
         *   cb(frame, (void *) 12345)
         */
        res = uvc_start_streaming(devh, &ctrl, cb, (void *) 12345, 0);

        if (res < 0) {
          uvc_perror(res, "start_streaming"); /* unable to start stream */
        } else {
          puts("Streaming...");

          /* enable auto exposure - see uvc_set_ae_mode documentation */
          puts("Enabling auto exposure ...");
          const uint8_t UVC_AUTO_EXPOSURE_MODE_AUTO = 2;
          res = uvc_set_ae_mode(devh, UVC_AUTO_EXPOSURE_MODE_AUTO);
          if (res == UVC_SUCCESS) {
            puts(" ... enabled auto exposure");
          } else if (res == UVC_ERROR_PIPE) {
            /* this error indicates that the camera does not support the full AE mode;
             * try again, using aperture priority mode (fixed aperture, variable exposure time) */
            puts(" ... full AE not supported, trying aperture priority mode");
            const uint8_t UVC_AUTO_EXPOSURE_MODE_APERTURE_PRIORITY = 8;
            res = uvc_set_ae_mode(devh, UVC_AUTO_EXPOSURE_MODE_APERTURE_PRIORITY);
            if (res < 0) {
              uvc_perror(res, " ... uvc_set_ae_mode failed to enable aperture priority mode");
            } else {
              puts(" ... enabled aperture priority auto exposure mode");
            }
          } else {
            uvc_perror(res, " ... uvc_set_ae_mode failed to enable auto exposure mode");
          }

          sleep(10); /* stream for 10 seconds */

          /* End the stream. Blocks until last callback is serviced */
          uvc_stop_streaming(devh);
          puts("Done streaming.");
        }
      }

      /* Release our handle on the device */
      uvc_close(devh);
      puts("Device closed");
    }

    /* Release the device descriptor */
    uvc_unref_device(dev);
  }

  /* Close the UVC context. This closes and cleans up any existing device handles,
   * and it closes the libusb context if one was not provided. */
  uvc_exit(ctx);
  puts("UVC exited");

  return 0;
}

