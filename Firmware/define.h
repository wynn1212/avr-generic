#define LED_NUM  1
#define INPUT  0
#define OUTPUT 1
#define HIGH 1
#define LOW  0
#define  byte unsigned char
#define LINE2 0xC0
#define LINE1 0x80
#define dT 30
#define pinlen 5
/* Begin of Define Port Direction */
#define XOR_RUNLED PORTC ^= 0x08
#define RS 7  
#define E 5
#define RW 6

/* End of Define Port Direction */


//#define Release

#ifdef Release
	//Change to NO_ISP mode
	//Use reset pin.
	//Important note, You have to manually set fuse bit 0x49,0x9F (Fuse_High,Fuse_Low) to work. suggestion, use HVPP
	//Uncheck RSTDISBL
	#define BEEP 22
#else
	//Important note, You have to manually set fuse bit 0xCA,0xDF (Fuse_High,Fuse_Low) to work. suggestion, use HVPP
	//Check RSTDISBL
	#define BEEP 6
#endif

