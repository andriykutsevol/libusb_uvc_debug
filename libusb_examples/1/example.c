
// sudo ./example 1133 2085
        // rally: 1133 2177

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>  // for strtol
#include <errno.h>

int fdnum_yuv;
int fdnum_bgr;


long vidi;
long pidi;
char* tmpptr;


unsigned int width_p;
unsigned int height_p;
int fmt_index;
int frame_index;

/** Converts an unaligned four-byte little-endian integer into an int32 */
#define DW_TO_INT(p) ((p)[0] | ((p)[1] << 8) | ((p)[2] << 16) | ((p)[3] << 24))
/** Converts an unaligned four-byte little-endian integer into an int32 */
#define DW_TO_INT(p) ((p)[0] | ((p)[1] << 8) | ((p)[2] << 16) | ((p)[3] << 24))
/** Converts an unaligned two-byte little-endian integer into an int16 */
#define SW_TO_SHORT(p) ((p)[0] | ((p)[1] << 8))


/** VideoStreaming interface descriptor subtype (A.6) */
enum uvc_vs_desc_subtype {
  UVC_VS_UNDEFINED = 0x00,
  UVC_VS_INPUT_HEADER = 0x01,
  UVC_VS_OUTPUT_HEADER = 0x02,
  UVC_VS_STILL_IMAGE_FRAME = 0x03,
  UVC_VS_FORMAT_UNCOMPRESSED = 0x04,
  UVC_VS_FRAME_UNCOMPRESSED = 0x05,
  UVC_VS_FORMAT_MJPEG = 0x06,
  UVC_VS_FRAME_MJPEG = 0x07,
  UVC_VS_FORMAT_MPEG2TS = 0x0a,
  UVC_VS_FORMAT_DV = 0x0c,
  UVC_VS_COLORFORMAT = 0x0d,
  UVC_VS_FORMAT_FRAME_BASED = 0x10,
  UVC_VS_FRAME_FRAME_BASED = 0x11,
  UVC_VS_FORMAT_STREAM_BASED = 0x12
};



/** VideoControl interface descriptor subtype (A.5) */
enum uvc_vc_desc_subtype {
  UVC_VC_DESCRIPTOR_UNDEFINED = 0x00,
  UVC_VC_HEADER = 0x01,
  UVC_VC_INPUT_TERMINAL = 0x02,
  UVC_VC_OUTPUT_TERMINAL = 0x03,
  UVC_VC_SELECTOR_UNIT = 0x04,
  UVC_VC_PROCESSING_UNIT = 0x05,
  UVC_VC_EXTENSION_UNIT = 0x06
};


/** UVC endpoint descriptor subtype (A.7) */
enum uvc_ep_desc_subtype {
  UVC_EP_UNDEFINED = 0x00,
  UVC_EP_GENERAL = 0x01,
  UVC_EP_ENDPOINT = 0x02,
  UVC_EP_INTERRUPT = 0x03
};


static struct libusb_device_handle *dev_handle = NULL;
static struct libusb_device *usb_dev = NULL;

int main(int argc, char **argv){


    vidi = strtol(argv[1],tmpptr,10);
    pidi = strtol(argv[2],tmpptr,10);

    //fmt_index = strtol(argv[3],tmpptr,10);
    //frame_index = strtol(argv[4],tmpptr,10);


    int rc;
    rc = libusb_init(NULL);
    if (rc < 0) {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(rc));
        exit(1);
    }

    dev_handle = libusb_open_device_with_vid_pid(NULL, vidi, pidi);

    if (!dev_handle) {
        fprintf(stderr, "Error finding USB device\n");
        goto out;
    }


    usb_dev = libusb_get_device(dev_handle) ;


    struct libusb_device_descriptor desc;

    if ( libusb_get_device_descriptor ( usb_dev, &desc ) != LIBUSB_SUCCESS ){
        fprintf(stderr, "Error getting Device Descriptor\n");
        goto out;
    }


    printf("desc.bLength: %d\n", desc.bLength);
    printf("desc.bDescriptorType: %d\n", desc.bDescriptorType);
    printf("desc.bcdUSB: %d\n", desc.bcdUSB);
    printf("desc.bDeviceClass: %d\n", desc.bDeviceClass);
    printf("desc.bDeviceSubClass: %d\n", desc.bDeviceSubClass);
    printf("desc.bDeviceProtocol: %d\n", desc.bDeviceProtocol);
    printf("desc.bMaxPacketSize0: %d\n", desc.bMaxPacketSize0);
    printf("desc.idVendor: %d\n", desc.idVendor);
    printf("desc.idProduct: %d\n", desc.idProduct);
    printf("desc.bcdDevice: %d\n", desc.bcdDevice);
    printf("desc.iManufacturer: %d\n", desc.iManufacturer);
    printf("desc.iProduct: %d\n", desc.iProduct);
    printf("desc.iSerialNumber: %d\n", desc.iSerialNumber);
    printf("desc.bNumConfigurations: %d\n", desc.bNumConfigurations);


    // https://www.beyondlogic.org/usbnutshell/usb5.shtml
    // The configuration descriptor specifies values such as 
    // the amount of power this particular configuration uses, 
    // if the device is self or bus powered and the number of interfaces it has. 
    // When a device is enumerated, the host reads the device descriptors 
    // and can make a decision of which configuration to enable. 
    // It can only enable one configuration at a time.


    printf("===================================\n");
    printf("===================================\n");


    struct libusb_config_descriptor *config;

    if (libusb_get_config_descriptor(usb_dev, 0, &config) != 0){
        fprintf(stderr, "Error getting Config Descriptor\n");
        goto out;
    }


    printf("config->bLength: %d\n", config->bLength);
    printf("config->bDescriptorType: %d\n", config->bDescriptorType);
    printf("config->wTotalLength: %d\n", config->wTotalLength);
    printf("config->bNumInterfaces: %d\n", config->bNumInterfaces);
    printf("config->bConfigurationValue: %d\n", config->bConfigurationValue);
    printf("config->iConfiguration: %d\n", config->iConfiguration);
    printf("config->bmAttributes: %d\n", config->bmAttributes);
    printf("config->MaxPower: %d\n", config->MaxPower);

    /** Array of interfaces supported by this configuration. The length of
     * this array is determined by the bNumInterfaces field. */
    printf("config->interface: %p\n", config->interface);

	/** Extra descriptors. If libusb encounters unknown configuration
	 * descriptors, it will store them here, should you wish to parse them. */
    printf("config->extra: %p\n", config->extra);


    /** Length of the extra descriptors, in bytes. Must be non-negative. */
    printf("config->extra_length: %d\n", config->extra_length);


    printf("===================================\n");
    printf("===================================\n");


    /* per interface */
    uint8_t got_interface = 0;
    int interface_idx;
    const struct libusb_interface *interface;

      /* per altsetting */
    int altsetting_idx;
    const struct libusb_interface_descriptor *if_desc;


      /* per endpoint */
    int endpoint_idx;
    const struct libusb_endpoint_descriptor *ep_desc;


    for (interface_idx = 0; !got_interface && interface_idx < config->bNumInterfaces; ++interface_idx) {

        interface = &config->interface[interface_idx];

        printf("======================================\n");
        printf("INTERFACE: %d \n", interface_idx);
        printf("======================================\n");


        /** Array of interface descriptors. The length of this array is determined
         * by the num_altsetting field. */
        printf("interface->altsetting: %p\n", interface->altsetting);
        printf("interface->num_altsetting: %d\n", interface->num_altsetting);


        for (altsetting_idx = 0; !got_interface && altsetting_idx < interface->num_altsetting; ++altsetting_idx) {

            if_desc = &interface->altsetting[altsetting_idx];

            printf("------------------------\n");
            printf("ALTSETTING(if_desc): %d, INTERFACE: %d\n", altsetting_idx, interface_idx); 

            printf("if_desc->bLength: %d\n", if_desc->bLength);
            printf("if_desc->bDescriptorType: %d\n", if_desc->bDescriptorType);
            printf("if_desc->bInterfaceNumber: %d\n", if_desc->bInterfaceNumber);
            printf("if_desc->bAlternateSetting: %d\n", if_desc->bAlternateSetting);
            printf("if_desc->bNumEndpoints: %d\n", if_desc->bNumEndpoints);
            printf("if_desc->bInterfaceClass: %d\n", if_desc->bInterfaceClass);
            printf("if_desc->bInterfaceSubClass: %d\n", if_desc->bInterfaceSubClass);
            printf("if_desc->bInterfaceProtocol: %d\n", if_desc->bInterfaceProtocol);
            printf("if_desc->iInterface: %d\n", if_desc->iInterface);

            /** Array of endpoint descriptors. This length of this array is determined
            * by the bNumEndpoints field. */
            printf("if_desc->endpoint: %p\n", if_desc->endpoint);

            /** Extra descriptors. If libusb encounters unknown interface descriptors,
	        * it will store them here, should you wish to parse them. */
            printf("if_desc->extra: %p\n", if_desc->extra);
            /** Length of the extra descriptors, in bytes. Must be non-negative. */
            printf("if_desc->extra_length: %d\n", if_desc->extra_length);    


            printf("LOOKING FOR CONTROL INTERFACE...\n");
            //if (altsetting_idx == 0){
                    if (if_desc->bInterfaceClass == 14 && if_desc->bInterfaceSubClass == 1){
                        printf("======================================\n");
                        printf("VIDEO CONTROL FOUND... PARSING...\n");
                        printf("======================================\n");

                        int video_control_idx = 0;

                        unsigned char *buffer_ci = if_desc->extra;
                        size_t buffer_left_ci = if_desc->extra_length;
                        size_t block_size_ci;
                        int descriptor_subtype;

                        while (buffer_left_ci >= 3){

                            block_size_ci = buffer_ci[0];

                            if (buffer_ci[1] != 36) { // not a CS_INTERFACE descriptor??
                                printf("ERROR: UVC_ERROR_INVALID_DEVICE\n");
                                return -1; // UVC_ERROR_INVALID_DEVICE;
                            }


                            descriptor_subtype = buffer_ci[2];

                            switch (descriptor_subtype) {
                            case UVC_VC_HEADER:

                                printf("---------UVC_VC_HEADER---------\n");
                                printf("uvc_parse_vc_header(dev, info, block, block_size);\n");

                                uint16_t bcdUVC = SW_TO_SHORT(&buffer_ci[3]);
                                printf("bcdUVC = SW_TO_SHORT(&buffer_ci[3]): %x\n", bcdUVC);

                                switch (bcdUVC){
                                case 0x0100:
                                    printf("dwClockFrequency = DW_TO_INT(buffer_ci + 7): %d\n", DW_TO_INT(buffer_ci + 7));
                                    break;
                                case 0x010a:
                                    printf("dwClockFrequency = DW_TO_INT(buffer_ci + 7): %d\n", DW_TO_INT(buffer_ci + 7));
                                    break;
                                case 0x0110:
                                    break;
                                default:
                                    printf("UVC_ERROR_NOT_SUPPORTED\n");
                                    return -1;
                                }

                                printf("uvc_scan_streaming(dev, info, block[i]); ...\n");
                                printf("uvc_parse_vs(dev, info, stream_if, buffer, block_size); ...\n");
                                break;
                            case UVC_VC_INPUT_TERMINAL:
                                printf("---------UVC_VC_INPUT_TERMINAL---------\n");
                                printf("uvc_parse_vc_input_terminal(dev, info, block, block_size);\n");

                                printf("bTerminalID = buffer_ci[3]: %d\n", buffer_ci[3]);
                                printf("wTerminalType = SW_TO_SHORT(&buffer_ci[4]): %d\n", SW_TO_SHORT(&buffer_ci[4]));
                                printf("wObjectiveFocalLengthMin = SW_TO_SHORT(&buffer_ci[8]): %d\n", SW_TO_SHORT(&buffer_ci[8]));
                                printf("wObjectiveFocalLengthMax = SW_TO_SHORT(&buffer_ci[10]): %d\n", SW_TO_SHORT(&buffer_ci[10]));
                                printf("wOcularFocalLength = SW_TO_SHORT(&buffer_ci[12]): %d\n", SW_TO_SHORT(&buffer_ci[12]));

                                uint64_t bmControls = 0;

                                for (int i = 14 + buffer_ci[14]; i >= 15; --i)
                                    bmControls = buffer_ci[i] + (bmControls << 8);
                                    printf("bmControls = buffer_ci[i] + (bmControls << 8): %d\n", bmControls);

                                break;
                            case UVC_VC_OUTPUT_TERMINAL:
                                printf("---------UVC_VC_OUTPUT_TERMINAL---------\n");
                                printf("---------nothing to do---------\n");
                                break;
                            case UVC_VC_SELECTOR_UNIT:
                                printf("---------UVC_VC_SELECTOR_UNIT---------\n");
                                printf("uvc_parse_vc_selector_unit(dev, info, block, block_size);\n");
                                printf("bUnitID = buffer_ci[3]: %d\n", buffer_ci[3]);
                                break;
                            case UVC_VC_PROCESSING_UNIT:
                                printf("---------UVC_VC_PROCESSING_UNIT---------\n");
                                printf("uvc_parse_vc_processing_unit(dev, info, block, block_size);\n");
                                printf("bUnitID = buffer_ci[3]: %d\n", buffer_ci[3]);
                                printf("bSourceID = buffer_ci[3]: %d\n", buffer_ci[4]);
                                
                                bmControls = 0;
                                for (int i = 7 + buffer_ci[7]; i >= 8; --i)
                                    bmControls = buffer_ci[i] + (bmControls << 8);
                                    printf("bmControls = buffer_ci[i] + (bmControls << 8): %d\n", bmControls);


                                break;
                            case UVC_VC_EXTENSION_UNIT:
                                printf("---------UVC_VC_EXTENSION_UNIT---------\n");
                                printf("uvc_parse_vc_extension_unit(dev, info, block, block_size);\n");
                                printf("bUnitID = buffer_ci[3]: %d\n", buffer_ci[3]);
                                printf("guidExtensionCode = buffer_ci[4]: %d\n", buffer_ci[4]);
                                int num_in_pins = buffer_ci[21];
                                printf("num_in_pins = buffer_ci[21]: %d\n", num_in_pins);
                                int size_of_controls = buffer_ci[22 + num_in_pins];
                                printf("size_of_controls = buffer_ci[22 + num_in_pins]: %d\n", size_of_controls);
                                const uint8_t *start_of_controls = &buffer_ci[23 + num_in_pins];
                                printf("start_of_controls = &buffer_ci[23 + num_in_pins]: %p\n", start_of_controls);

                                bmControls = 0;
                                for (int i = size_of_controls - 1; i >= 0; --i)
                                    bmControls = start_of_controls[i] + (bmControls << 8);
                                    printf("bmControls = buffer_ci[i] + (bmControls << 8): %d\n", bmControls);


                                break;
                            default:
                                //ret = UVC_ERROR_INVALID_DEVICE;
                            }




                            buffer_left_ci -= block_size_ci;
                            buffer_ci += block_size_ci;

                        }

                        
                        printf("======================================\n");
                        printf("END VIDEO CONTROL PARSING.\n");
                        printf("======================================\n");

                    } // Video, Control

            //}



            for (endpoint_idx = 0;  endpoint_idx < if_desc->bNumEndpoints; ++endpoint_idx) {

                ep_desc = &if_desc->endpoint[endpoint_idx];


                printf("------------------------\n");
                printf("ENDPOINT: endpoint_idx %d, ALTSETTING(if_desc): %d, INTERFACE: %d\n", endpoint_idx, altsetting_idx, interface_idx); 

                printf("ep_desc->bLength: %d\n", ep_desc->bLength);
                printf("ep_desc->bDescriptorType: %d\n", ep_desc->bDescriptorType);
                printf("ep_desc->bEndpointAddress: %d\n", ep_desc->bEndpointAddress);
                printf("ep_desc->bmAttributes: %d\n", ep_desc->bmAttributes);
                printf("ep_desc->wMaxPacketSize: %d\n", ep_desc->wMaxPacketSize);
                printf("ep_desc->bInterval: %d\n", ep_desc->bInterval);
                printf("ep_desc->bRefresh: %d\n", ep_desc->bRefresh);
                printf("ep_desc->bSynchAddress: %d\n", ep_desc->bSynchAddress);

                /** Extra descriptors. If libusb encounters unknown endpoint descriptors,
                 * it will store them here, should you wish to parse them. */  
                printf("ep_desc->extra: %p\n", ep_desc->extra);
                /** Length of the extra descriptors, in bytes. Must be non-negative. */
                printf("ep_desc->extra_length: %d\n", ep_desc->extra_length);                


                if (ep_desc->extra_length > 0){

                    unsigned char *buffer_ep = ep_desc->extra;
                    size_t buffer_left_ep = ep_desc->extra_length;
                    size_t block_size_ep;
                    int descriptor_subtype_ep;

                    int extra_endpoint_idx = 0;
                    while (buffer_left_ep >= 3) {

                        printf("---------EXTRA EP : %d---------\n", extra_endpoint_idx);

                        block_size_ep = buffer_ep[0];
                        printf("block_size: %d\n", block_size_ep);
                        printf("buffer_left: %d\n", buffer_left_ep);
                        extra_endpoint_idx += 1;

                        buffer_left_ep -= block_size_ep;
                        buffer_ep += block_size_ep;  

                    }

                }

                printf("-----LOOKING FOR ENDPOINT COMPANION DESCTIPTOR ...-----\n"); 
                size_t endpoint_bytes_per_packet = 0;
                struct libusb_ss_endpoint_companion_descriptor *ep_comp = 0;

                    // /** \ingroup libusb_desc
                    //  * A structure representing the superspeed endpoint companion
                    //  * descriptor. This descriptor is documented in section 9.6.7 of
                    //  * the USB 3.0 specification. All multiple-byte fields are represented in
                    //  * host-endian format.
                    //  */
                    // struct libusb_ss_endpoint_companion_descriptor {
                    //     /** Size of this descriptor (in bytes) */
                    //     uint8_t  bLength;

                    //     /** Descriptor type. Will have value
                    //      * \ref libusb_descriptor_type::LIBUSB_DT_SS_ENDPOINT_COMPANION in
                    //      * this context. */
                    //     uint8_t  bDescriptorType;

                    //     /** The maximum number of packets the endpoint can send or
                    //      *  receive as part of a burst. */
                    //     uint8_t  bMaxBurst;

                    //     /** In bulk EP: bits 4:0 represents the maximum number of
                    //      *  streams the EP supports. In isochronous EP: bits 1:0
                    //      *  represents the Mult - a zero based value that determines
                    //      *  the maximum number of packets within a service interval  */
                    //     uint8_t  bmAttributes;

                    //     /** The total number of bytes this EP will transfer every
                    //      *  service interval. Valid only for periodic EPs. */
                    //     uint16_t wBytesPerInterval;
                    // };


                libusb_get_ss_endpoint_companion_descriptor(NULL, ep_desc, &ep_comp); 
                if (ep_comp)
                {
                    printf("-----ENDPOINT COMPANION DESCTIPTOR FOUND. PARSING ...-----\n");
                    printf("ep_comp->bLength: %d\n", ep_comp->bLength);
                    printf("ep_comp->bDescriptorType: %d\n", ep_comp->bDescriptorType);
                    printf("ep_comp->bMaxBurst: %d\n", ep_comp->bMaxBurst);
                    printf("ep_comp->bmAttributes: %d\n", ep_comp->bmAttributes);
                    printf("ep_comp->wBytesPerInterval: %d\n", ep_comp->wBytesPerInterval);

                    endpoint_bytes_per_packet = ep_comp->wBytesPerInterval;
                    libusb_free_ss_endpoint_companion_descriptor(ep_comp);

                    printf("endpoint_bytes_per_packet = ep_comp->wBytesPerInterval: %d\n", endpoint_bytes_per_packet);

                    printf("-----END COMPANION DESCTIPTOR PARSING ...-----\n");

                    break;
                }else{
                    endpoint_bytes_per_packet = ep_desc->wMaxPacketSize;
                    endpoint_bytes_per_packet = (endpoint_bytes_per_packet & 0x07ff) * (((endpoint_bytes_per_packet >> 11) & 3) + 1);

                    printf("endpoint_bytes_per_packet = ep_desc->wMaxPacketSize: %d\n", ep_desc->wMaxPacketSize);
                    printf("endpoint_bytes_per_packet = (endpoint_bytes_per_packet & 0x07ff) * (((endpoint_bytes_per_packet >> 11) & 3) + 1): %d\n", endpoint_bytes_per_packet);

                }


            }


            if (if_desc->extra_length > 0){

                unsigned char *buffer = if_desc->extra;
                size_t buffer_left = if_desc->extra_length;
                size_t block_size;
                int descriptor_subtype;
                int extra_interface_idx = 0;

                int i_UVC_VS_INPUT_HEADER = 0;
                int i_UVC_VS_OUTPUT_HEADER = 0;
                int i_UVC_VS_STILL_IMAGE_FRAME = 0;
                int i_UVC_VS_FORMAT_UNCOMPRESSED = 0;
                int i_UVC_VS_FORMAT_MJPEG = 0;
                int i_UVC_VS_FRAME_UNCOMPRESSED = 0;
                int i_UVC_VS_FRAME_MJPEG = 0;
                int i_UVC_VS_FORMAT_MPEG2TS = 0;
                int i_UVC_VS_FORMAT_DV = 0;
                int i_UVC_VS_COLORFORMAT = 0;
                int i_UVC_VS_FORMAT_FRAME_BASED = 0;
                int i_UVC_VS_FRAME_FRAME_BASED = 0;
                int i_UVC_VS_FORMAT_STREAM_BASED = 0;


                while (buffer_left >= 3) {
                    
                    block_size = buffer[0];

                    printf("--------EXTRA INTERFACE :%d: ALTSETTING(if_desc): %d, INTERFACE: %d--------\n", extra_interface_idx, altsetting_idx, interface_idx);
                    printf("block_size: %d\n", block_size);
                    printf("buffer_left: %d\n", buffer_left);
                    extra_interface_idx += 1;


                    descriptor_subtype = buffer[2];
                    printf("descriptor_subtype: %d\n", descriptor_subtype);


                    switch (descriptor_subtype) {

                        case UVC_VS_INPUT_HEADER:
                            printf("UVC_VS_INPUT_HEADER: %d\n", i_UVC_VS_INPUT_HEADER);
                            printf("uvc_parse_vs_input_header(stream_if, buffer, block_size); \n");
                            i_UVC_VS_INPUT_HEADER += 1;
                            break;
                        case UVC_VS_OUTPUT_HEADER:
                            printf("UVC_VS_OUTPUT_HEADER: %d\n", i_UVC_VS_OUTPUT_HEADER);
                            printf("unsupported descriptor subtype VS_OUTPUT_HEADER\n");
                            i_UVC_VS_INPUT_HEADER += 1;
                            break;
                        case UVC_VS_STILL_IMAGE_FRAME:
                            printf("UVC_VS_STILL_IMAGE_FRAME: %d\n", i_UVC_VS_STILL_IMAGE_FRAME);
                            printf("uvc_parse_vs_still_image_frame(stream_if, block, block_size);\n");
                            i_UVC_VS_STILL_IMAGE_FRAME += 1;
                            break;
                        case UVC_VS_FORMAT_UNCOMPRESSED:
                            // https://learn.microsoft.com/en-us/windows-hardware/drivers/stream/uvc-camera-implementation-guide

                            printf("UVC_VS_FORMAT_UNCOMPRESSED: %d\n", i_UVC_VS_FORMAT_UNCOMPRESSED);
                            printf("uvc_parse_vs_format_uncompressed(stream_if, block, block_size);\n");
                            printf("bDescriptorSubtype = buffer[2]: %d\n", buffer[2]);
                            printf("bFormatIndex = buffer[3]: %d\n", buffer[3]);
                            printf("bBitsPerPixel = buffer[21]: %d\n", buffer[21]);
                            printf("bBitsPerPixel = buffer[21]: %d\n", buffer[21]);
                            printf("bDefaultFrameIndex = buffer[22]: %d\n", buffer[22]);
                            printf("bAspectRatioX = buffer[23]: %d\n", buffer[23]);
                            printf("bAspectRatioY = buffer[24]: %d\n", buffer[24]);
                            i_UVC_VS_FORMAT_UNCOMPRESSED += 1;
                            break;
                        case UVC_VS_FORMAT_MJPEG:
                            printf("UVC_VS_FORMAT_MJPEG: %d\n", i_UVC_VS_FORMAT_MJPEG);
                            printf("uvc_parse_vs_format_mjpeg(stream_if, block, block_size);\n");
                            i_UVC_VS_FORMAT_MJPEG += 1;
                            break;
                        case UVC_VS_FRAME_UNCOMPRESSED:
                            printf("UVC_VS_FRAME_UNCOMPRESSED: %d\n", i_UVC_VS_FRAME_UNCOMPRESSED);
                            i_UVC_VS_FRAME_UNCOMPRESSED += 1;
                        case UVC_VS_FRAME_MJPEG:
                            printf("UVC_VS_FRAME_MJPEG: %d\n", i_UVC_VS_FRAME_MJPEG);
                            printf("uvc_parse_vs_frame_uncompressed(stream_if, block, block_size);\n");
                            printf("bDescriptorSubtype = buffer[2]: %d\n", buffer[2]);
                            printf("bFrameIndex = buffer[3]: %d\n", buffer[3]);
                            printf("bmCapabilities = buffer[4]: %d\n", buffer[4]);
                            printf("wWidth = buffer[5] + (buffer[6] << 8): %d\n", buffer[5] + (buffer[6] << 8));
                            printf("wHeight = buffer[7] + (buffer[8] << 8): %d\n", buffer[7] + (buffer[8] << 8));
                            printf("dwMinBitRate = DW_TO_INT(&buffer[9]): %d\n", DW_TO_INT(&buffer[9]));
                            printf("dwMaxBitRate = DW_TO_INT(&buffer[13]): %d\n", DW_TO_INT(&buffer[13]));
                            printf("dwMaxVideoFrameBufferSize = DW_TO_INT(&buffer[17]): %d\n", DW_TO_INT(&buffer[17]));
                            printf("dwDefaultFrameInterval = DW_TO_INT(&buffer[21]): %d\n", DW_TO_INT(&buffer[21]));
                            printf("bFrameIntervalType = buffer[25]: %d\n", buffer[25]);

                            if (buffer[25] == 0) {
                                printf("dwMinFrameInterval = DW_TO_INT(&block[26]: %d\n", DW_TO_INT(&buffer[26]));
                                printf("dwMaxFrameInterval = DW_TO_INT(&block[30]: %d\n", DW_TO_INT(&buffer[30]));
                                printf("dwFrameIntervalStep = DW_TO_INT(&block[34]: %d\n", DW_TO_INT(&buffer[34]));
                            } else {
                                const unsigned char *p;
                                uint32_t *intervals = calloc(buffer[25] + 1, sizeof(uint32_t));
                                p = &buffer[26];

                                for (int i = 0; i < buffer[25]; ++i) {
                                    intervals[i] = DW_TO_INT(p);
                                    p += 4;
                                    printf("intervals[%d]: %d\n", i, intervals[i]);
                                }
                                intervals[buffer[25]] = 0;
                                printf("intervals[%d]: %d\n", buffer[25], intervals[buffer[25]]);

                            }
                            

                            i_UVC_VS_FRAME_MJPEG += 1;
                            break;
                        case UVC_VS_FORMAT_MPEG2TS:
                            printf("UVC_VS_FORMAT_MPEG2TS: %d\n", i_UVC_VS_FORMAT_MPEG2TS);
                            i_UVC_VS_FORMAT_MPEG2TS += 1;
                            break;
                        case UVC_VS_FORMAT_DV:
                            printf("VS_FORMAT_DV unsupported descriptor subtype: %d\n", i_UVC_VS_FORMAT_DV);
                            i_UVC_VS_FORMAT_DV += 1;
                            break;
                        case UVC_VS_COLORFORMAT:
                            printf("VS_COLORFORMAT unsupported descriptor subtype: %d\n", i_UVC_VS_COLORFORMAT);
                            i_UVC_VS_COLORFORMAT += 1;
                            break;
                        case UVC_VS_FORMAT_FRAME_BASED:
                            printf("UVC_VS_FORMAT_FRAME_BASED: %d\n", i_UVC_VS_FORMAT_FRAME_BASED);
                            printf("uvc_parse_vs_frame_format ( stream_if, block, block_size );\n");
                            i_UVC_VS_FORMAT_FRAME_BASED += 1;
                            break;
                        case UVC_VS_FRAME_FRAME_BASED:
                            printf("UVC_VS_FRAME_FRAME_BASED: %d\n", i_UVC_VS_FRAME_FRAME_BASED);
                            printf("uvc_parse_vs_frame_frame ( stream_if, block, block_size );\n");

                            printf("bDescriptorSubtype = buffer[2]: %d\n", buffer[2]);
                            printf("bFrameIndex = buffer[3]: %d\n", buffer[3]);
                            printf("bmCapabilities = buffer[4]: %d\n", buffer[4]);
                            printf("wWidth = buffer[5] + (buffer[6] << 8): %d\n", buffer[5] + (buffer[6] << 8));
                            printf("wHeight = buffer[7] + (buffer[8] << 8): %d\n", buffer[7] + (buffer[8] << 8));
                            printf("dwMinBitRate = DW_TO_INT(&buffer[9]): %d\n", DW_TO_INT(&buffer[9]));
                            printf("dwMaxBitRate = DW_TO_INT(&buffer[13]): %d\n", DW_TO_INT(&buffer[13]));
                            printf("dwDefaultFrameInterval = DW_TO_INT(&buffer[17]): %d\n", DW_TO_INT(&buffer[17]));
                            printf("bFrameIntervalType = buffer[21]: %d\n", buffer[21]);
                            printf("dwBytesPerLine = DW_TO_INT(&buffer[22]): %d\n", DW_TO_INT(&buffer[22]));

                            i_UVC_VS_FRAME_FRAME_BASED += 1;
                            break;
                        case UVC_VS_FORMAT_STREAM_BASED:
                            printf("VS_FORMAT_STREAM_BASED unsupported descriptor subtype: %d\n", i_UVC_VS_FORMAT_STREAM_BASED);
                            i_UVC_VS_FORMAT_STREAM_BASED += 1;
                            break;
                        default:
                            /** @todo handle JPEG and maybe still frames or even DV... */
                            //UVC_DEBUG("unsupported descriptor subtype: %d",descriptor_subtype);
                            printf("unsupported descriptor subtype: %d\n", descriptor_subtype);
                            break;


                    }

                    buffer_left -= block_size;
                    buffer += block_size;                

                }

            }

        }

    printf("======================================\n");
    printf("END INTERFACE: %d \n", interface_idx);
    printf("======================================\n");

    }


    printf("hello\n");


out:
    if (dev_handle)
        libusb_close(dev_handle);
    libusb_exit(NULL);
    return rc;

}





// INTERFACE
//    ALTSETTING
//        CONTROL INTERFACE
//        ENDPOINT 
//          EXTRA EP
//          ENDPOINT COMPANION
//        EXTRA INTERFACE
// INTERFACE
//    ALTSETTING
//        CONTROL INTERFACE
//        ENDPOINT 
//          EXTRA EP
//          ENDPOINT COMPANION
//        EXTRA INTERFACE