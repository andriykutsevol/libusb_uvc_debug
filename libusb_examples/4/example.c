
// sudo ./example 1133 2085     // c270
// sudo ./example 10549 6        // megawell

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>  // for strtol
#include <errno.h>


#include <pthread.h>
#include <signal.h>



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






//================================================================
//================================================================

static unsigned long num_bytes = 0, num_xfer = 0;


static void LIBUSB_CALL cb_xfr(struct libusb_transfer *xfr)
{
	
  printf("yyyyyyyyyy\n");
  
  int i;

	if (xfr->status != LIBUSB_TRANSFER_COMPLETED) {
		printf("transfer status %d\n", xfr->status);
		libusb_free_transfer(xfr);
		exit(3);
	}

	if (xfr->type == LIBUSB_TRANSFER_TYPE_ISOCHRONOUS) {
		for (i = 0; i < xfr->num_iso_packets; i++) {
			struct libusb_iso_packet_descriptor *pack = &xfr->iso_packet_desc[i];

			if (pack->status != LIBUSB_TRANSFER_COMPLETED) {
				printf("Error: pack %d status %d\n", i, pack->status);
				exit(5);
			}

			printf("pack%d length:%u, actual_length:%u\n", i, pack->length, pack->actual_length);
		}
	}

	printf("length:%u, actual_length:%u\n", xfr->length, xfr->actual_length);
	for (i = 0; i < xfr->actual_length; i++) {
		printf("%02x", xfr->buffer[i]);
		if (i % 16)
			printf("\n");
		else if (i % 8)
			printf("  ");
		else
			printf(" ");
	}
	num_bytes += xfr->actual_length;
	num_xfer++;

	if (libusb_submit_transfer(xfr) < 0) {
		printf("error re-submitting URB\n");
		exit(1);
	}
}





//================================================================
//================================================================

long vidi;
long pidi;
char* tmpptr;

static struct libusb_device_handle *devh = NULL;

static volatile sig_atomic_t do_exit = 0;


int main(int argc, char **argv){


  vidi = strtol(argv[1],tmpptr,10);
  pidi = strtol(argv[2],tmpptr,10);

  //fmt_index = strtol(argv[3],tmpptr,10);
  //frame_index = strtol(argv[4],tmpptr,10);


  //================================================================
  //================================================================

  uint8_t stream_bInterfaceNumber = 1;


  int rc;

  rc = libusb_init(NULL);
 
	if (rc < 0) {
		printf("Error initializing libusb: %s\n", libusb_error_name(rc));
		exit(1);
	}



	devh = libusb_open_device_with_vid_pid(NULL, vidi, pidi);
	if (!devh) {
		printf("Error finding USB device\n");
		goto out;
	} 


  rc = libusb_detach_kernel_driver(devh, stream_bInterfaceNumber);
  if (rc == UVC_SUCCESS || rc == LIBUSB_ERROR_NOT_FOUND || rc== LIBUSB_ERROR_NOT_SUPPORTED) {
      printf("claiming Stream interface %d\n", stream_bInterfaceNumber);
  }



  rc = libusb_claim_interface(devh, stream_bInterfaceNumber);
	if (rc < 0) {
		printf("Error straming interface: %s\n", libusb_error_name(rc));
		goto out;
	} 


  //================================================================
  //================================================================

  uint8_t bEndpointAddres = 129;
  size_t packets_per_transfer = 32;
  size_t total_transfer_size = 97920;
  static uint8_t buf[97920];

  static struct libusb_transfer *xfr;

	xfr = libusb_alloc_transfer(packets_per_transfer);
	if (!xfr) {
    printf("zzzzzzzzzzzzzzzzzzzz\n");
    printf("Error: libusb_alloc_transfer()\n");
    sleep(1);
		errno = ENOMEM;
		return -1;
	}


		libusb_fill_iso_transfer(xfr, devh, bEndpointAddres, 
        buf,
				sizeof(buf), 
        packets_per_transfer, 
        cb_xfr, NULL, 0);


    libusb_submit_transfer(xfr);


		libusb_set_iso_packet_lengths(xfr, sizeof(buf)/packets_per_transfer);

  //================================================================
  //================================================================


	while (!do_exit) {
		rc = libusb_handle_events(NULL);
    printf("while\n");
		if (rc != LIBUSB_SUCCESS)
      printf("zzzzzzzzzzzzzzzzzzzz\n");
      printf("Error: libusb_handle_events()\n");
      sleep(1);
			break;
	}  


  //================================================================
  //================================================================



out:
	if (devh)
		libusb_close(devh);
	libusb_exit(NULL);
	return rc;


}