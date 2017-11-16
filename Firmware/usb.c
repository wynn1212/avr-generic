/* The following variables store the status of the current data transfer */
static uchar    currentAddress;
static uchar    bytesRemaining;

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 * E.X. PC <= Atmega8
 */
 unsigned char i; 
 #define BASE 0x30
uchar   usbFunctionRead(uchar *data, uchar len){
	unsigned char i;
	if(len > bytesRemaining)len = bytesRemaining;
	if(currentAddress == 0){
		for ( i=0;i<8;i++) *(data+i) = OUTdata[i];
   }
   currentAddress += len;
   bytesRemaining -= len;
   
   return len;
}
/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 * E.X. PC => Atmega8
 */
uchar   usbFunctionWrite(uchar *data, uchar len){
	//static uchar tempB,tempD;
	if(bytesRemaining == 0)
		return 1;               /* end of transfer */
	if(len > bytesRemaining)
		len = bytesRemaining;
	if(currentAddress == 0){   	
		online = 1;
		for ( i=0; i<8; i++ ) INdata[i] = data[i];
	}
   currentAddress += len;
   bytesRemaining -= len;
   return bytesRemaining == 0;	/* return 1 if this was the last chunk */
}

usbMsgLen_t usbFunctionSetup(uchar data[8]){
	usbRequest_t    *rq = (void *)data;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
		if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
			/* since we have only one report type, we can ignore the report-ID */
			bytesRemaining = 8;
			currentAddress = 0;
			return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
		}else if(rq->bRequest == USBRQ_HID_SET_REPORT){
			/* since we have only one report type, we can ignore the report-ID */
			bytesRemaining = 8;
			currentAddress = 0;
			return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
		}
	}else{
		/* ignore vendor type requests, we don't use any */
	}
	return 0;
}
/* ----------------------------- USB interface ----------------------------- */
PROGMEM const char usbHidReportDescriptor[22] = {	/* USB report descriptor */
	0x06, 0x00, 0xff,								// USAGE_PAGE (Generic Desktop)
	0x09, 0x01,										// USAGE (Vendor Usage 1)
	0xa1, 0x01,										// COLLECTION (Application)
	0x15, 0x00,										//   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,								//   LOGICAL_MAXIMUM (255)
	0x75, 0x08,										//   REPORT_SIZE (8)
	0x95, 0x80,										//   REPORT_COUNT (128)
	0x09, 0x00,										//   USAGE (Undefined)
	0xb2, 0x02, 0x01,								//   FEATURE (Data,Var,Abs,Buf)
	0xc0											// END_COLLECTION
};
