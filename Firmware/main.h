// Initialization require header files.
#include "define.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.c"
#include "usbdrv/oddebug.c"        /* This is also an example for using debug macros */

uchar GetKeyPressed( void );

/* Begin of 办跑计 */
char strE[32];
unsigned int timer[10];
uchar bKeybusy = 0;
                      
unsigned char online = 0;
unsigned char cmd,CmdOut,flag_close;
unsigned char station_no;
uchar INdata[8];
uchar OUTdata[8];
uchar temp_timer[6];                  //PC
unsigned int t1cnt,vbf;
byte  LCDbuff[2][16];		
byte  bLCDref = 0;			//0:no refresh in need,  1:refresh required.
const char HexData[16] = "0123456789ABCDEF";

void System_init(void);
void init_timer(void);
void show_logo1(void);
void show_logo2(void);
void lb(int f);
void sb(int f);
void cb(void);
void lcdclear(void);
void lcdclear1(void);
void lcdclear2(void);
void keypadmain(char ch);
void job_1s(void);
void job_500ms(void);
void vbmode(void);

/* End of 办跑计 */

/* Begin of bool */
char B_wait200ms = 0;
char B_wait200msreset = 0;
char B_buzzon = 0;
/* End of bool */
uchar key;
unsigned int Hz = 1000;//Temp
