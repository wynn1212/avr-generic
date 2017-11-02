
/*
This example should run on most AVRs with only little changes. No special
hardware resources except INT0 are used. You may have to change usbconfig.h for
different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
at least be connected to INT0 as well.
*/
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */
#include "lcd.h" 
#include "lcd.c" 

#define LED_NUM  1

/* 全域變數 */
uchar scan_line, scan_in, scan_value, scan_last,count2;
uchar station_number, key;
uchar disp_mode, logo_flag ; 
unsigned int timer[4];
uchar pixels[3];                      
uchar hh,mm,ss,online;
uchar temp_timer[6];                  //PC

uchar keyboard_scan(void);
void process_keyin(uchar value);
void process_show_station(void);
void process_show(void);
void setPixelColor( uchar number, uchar color_r, uchar color_g, uchar color_b);
void show_logo1(void);
void show_logo2(void);

/* ----------------------------- USB interface ----------------------------- */
PROGMEM const char usbHidReportDescriptor[22] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};


/* The following variables store the status of the current data transfer */
static uchar    currentAddress;
static uchar    bytesRemaining;

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
   if(len > bytesRemaining)
      len = bytesRemaining;
 	 if(currentAddress == 0)
 	 {
    
    *(data)   = key;		                            
    *(data+1) = 0;		
 		*(data+2) = 0;				
 		*(data+3) = 0;			 		
 		*(data+4) = 0;				 		 		
 		*(data+5) = 0;		// Reserve (zero)
 		*(data+6) = 0;		// Reserve (zero)
 		*(data+7) = 0;		// Reserve (zero)
   }
   currentAddress += len;
   bytesRemaining -= len;
   
   return len;
}

/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
// 	 static uchar tempB,tempD;
	 
   if(bytesRemaining == 0)
      return 1;               /* end of transfer */
   if(len > bytesRemaining)
      len = bytesRemaining;
      
   if(currentAddress == 0)
   {   	
    	if((*(data+1) & 0x20) == 0x20)
    	{
    	   
    	} 
    	else if(*(data+1) == 0x01)      /* 更新顯示時間 */
    	{    	 
         temp_timer[0] = *(data+2);  //時
         temp_timer[1] = *(data+3);  //
         temp_timer[2] = *(data+4);  //分
         temp_timer[3] = *(data+5);  //
         temp_timer[4] = *(data+6);  //秒
         temp_timer[5] = *(data+7);  //    
         hh = temp_timer[0] * 10 + temp_timer[1];
         mm = temp_timer[2] * 10 + temp_timer[3];
         ss = temp_timer[4] * 10 + temp_timer[5];
         online = 1;                                                                                                                                 		 	     		 
    	}    	    			     
    	else if(*(data+1) == 0x04)      /* 設定STATION NUMBER */
    	{    	 
    	   station_number = *(data+2);
    	}	      	
    	else if(*(data+1) == 0x03)      /* 程式結束 */
    	{    	 
         online = 0;                                                                                                                                  		 	     		     	             	                               		 	     		 
    	}	    	
	
 	}
   currentAddress += len;
   bytesRemaining -= len;
   return bytesRemaining == 0; /* return 1 if this was the last chunk */
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
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
/* ------------------------------------------------------------------------- */
ISR (TIMER0_OVF_vect)                       // timer0 overflow interrupt
{
   uchar   i;
   // event to be exicuted every 5ms here
   TCNT0 = 22;  
      
   for(i=0;i<4;i++)
   {
      if(timer[i]>0)
         timer[i]--;           
   }    
}

int main(void)
{	
	 uchar   i;	
   wdt_enable(WDTO_1S);
   /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
   */
     
   /* IO SETUP */  
   {
     	                //   DDB7   DDB6   DDB5   DDB4   DDB3   DDB2   DDB1   DDB0
     	DDRB = 0x3f;    //    0      0      1      1      1      1      1      1
     	                //    x      x     out    out    out    out    out    out
                      //  PORTB7 PORTB6 PORTB5 PORTB4 PORTB3 PORTB2 PORTB1 PORTB0
     	PORTB = 0x05;   //    0      0      0      0      0      0      0      0
  
     
     				          //    -     DDC6   DDC5   DDC4   DDC3   DDC2   DDC1   DDC0
     	DDRC = 0x78;    //    0      1      1      1      1      0      0      0  
     					        //  (Read)  out    out    out    out    in      in    in
                      //    -    PORTC6 PORTC5 PORTC4 PORTC3 PORTC2 PORTC1 PORTC0							 
      PORTC = 0x07;   //    0      0      0      0      0      0      0      0
  
     	                //   DDD7   DDD6   DDD5   DDD4   DDD3   DDD2   DDD1   DDD0
     	DDRD = 0xeb;    //    1      1      1      0      1      0      1      1 
     					        //   out    out    out    D-     out     D+    out    out
     	                //  PORTD7 PORTD6 PORTD5 PORTD4 PORTD3 PORTD2 PORTD1 PORTD0
     	PORTD = 0x0;    //    0      0      0      0      0      0      0      0  
   }
	
   usbInit();
   usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
	 i = 0;
   while(--i)              /* fake USB disconnect for > 250 ms */
   {             
      wdt_reset();
      _delay_ms(1);
   }
   usbDeviceConnect();
   sei();
   
   TIMSK |= (1 << TOIE0);
   TCCR0 |= (1 << CS02);             // set prescaler to 256 and start the timer 
   TCNT0 = 128; 
      
   Init_LCD();                        /* LCD初始設定 */ 
               
   /* 變數初始設定 */ 
   scan_line = 0;          //0-2   
 	 station_number = 105;    
 	 count2 = 0;
   logo_flag  = 0;
   scan_last = 0xff;
   disp_mode = 0;
   hh = 0; mm = 0; ss = 0, online = 0;
   key = 0;
   
   DISP_Str(0x88,"WELCOME ");      //初始顯示                 
   DISP_Str(0xC0,"        ");      
     
   for(i=0;i<4;i++)
      timer[i] = 0;      

   //PWM SETUP
   TCNT0 = 128;    
   
   OCR2 = 128;                // set PWM for 50% duty cycle
   TCCR2 |= (1 << COM21);     // set non-inverting mode
   TCCR2 |= (1 << WGM21) | (1 << WGM20);     // set fast PWM Mode
//   TCCR2 |= (1 << CS21);   
   TCCR2 |= (1 << CS20);   
//   TCCR2 |= (1 << CS21);   
//   TCCR2 |= (1 << CS22);         
   
   for(;;)          /* main event loop */
   {                
      wdt_reset();
      usbPoll();		
		
  		if(timer[0] ==0)                          //5ms x 6 = 30ms, 鍵盤掃描時間
  		{       			                                   
  		   timer[0] = 6;    
  		   
           scan_value = keyboard_scan();         
           if(scan_value < 10) 
           {	      
              process_keyin(scan_value);    		   
              
           }   
  		}   	
        
      if(timer[1] ==0)                          //5ms x 100 = 0.5s 上數
      {
         timer[1] = 200; 
         
         if(disp_mode == 0)
         {
            count2++;
            count2 = count2 % 120;          
            DISP_Str(0x88,"WELCOME ");             //顯示站別                           
            DISP_Chr(0xCA, 0x30 | (count2/100));
            i = count2 % 100;
            DISP_Chr(0xCB, 0x30 | (i/10));
            DISP_Chr(0xCC, 0x30 | (i%10));     
            
            if(logo_flag >0 )
            {
            	logo_flag = 0;
              show_logo1();    	            	
            }	
            else
            {
            	logo_flag = 1;
              show_logo2();    	            	            	
            }	
         }	                                    
      }  

      if(timer[2] ==0)                          //5ms x 200 = 1s 上數 
      {         
         if((disp_mode == 1)&&(online == 0))
         {
         	  timer[2] = 200;
         	  ss++;
         	  if(ss>=60)
         	  {
         	     ss = 0;
         	     mm++;
         	     if(mm >= 60)
         	     {
         	        mm = 0;
         	        hh++;	
         	        if(hh >= 24)
         	           hh =0;   	
         	     }	         	     	
         	  }   	
         	
            DISP_Str(0x80,"   Time   :  :");	
            DISP_Chr(0x88, 0x30 | (hh/10));
            DISP_Chr(0x89, 0x30 | (hh%10));         	
            DISP_Chr(0x8B, 0x30 | (mm/10));
            DISP_Chr(0x8C, 0x30 | (mm%10));         	
            DISP_Chr(0x8E, 0x30 | (ss/10));
            DISP_Chr(0x8F, 0x30 | (ss%10));         	
         	  process_show_station();
         }
         else if((disp_mode == 1)&&(online == 1))  	         
         {
         	  timer[2] = 40;
            DISP_Str(0x80,"On Time   :  :");	
            DISP_Chr(0x88, 0x30 | temp_timer[0]);     //更新顯示時間           
            DISP_Chr(0x89, 0x30 | temp_timer[1]);         
            DISP_Chr(0x8B, 0x30 | temp_timer[2]);
            DISP_Chr(0x8C, 0x30 | temp_timer[3]);
            DISP_Chr(0x8E, 0x30 | temp_timer[4]);
            DISP_Chr(0x8F, 0x30 | temp_timer[5]);
         	  process_show_station();
         }	
      }            
               
   }
   return 0;
}


/* ------------------------------------------------------------------------- */
uchar keyboard_scan(void)                  //返回值=0xff(無按鍵輸入), (1-7 , * = 8, # = 9) 
{
   uchar value, i;

   value   = 0xff;
      
   for(i=0;i<3;i++)
   {
      //掃描線更換               
      PORTC = PORTC | 0x38;     
      
      if(i == 0)
      {
         scan_line = 0;
         PORTC = PORTC & 0xf7;       //PC3     
      }         
      else if(i == 1)
      {
         scan_line = 1;
         PORTC = PORTC & 0xEF;       //PC4      
      }   
      else if(i == 2)
      {
         scan_line = 2;
         PORTC = PORTC & 0xDF;       //PC5
      } 

      _delay_us(10);
      scan_in = PINC & 0x07;            
            
      if(scan_in != 0x07)          //有按鍵動作//
      {      	       	 
         scan_in = (~scan_in)& 0x07;  
         
         if (scan_in == 0x01)
            scan_last = 1;
         else if (scan_in == 0x02)
            scan_last = 2;
         else if (scan_in == 0x04)
            scan_last = 3;
         
         scan_last = scan_last + (scan_line * 3);   
                              
         return 0xff;                     
      }   
      else
      {  
         if(i>=2) 
         {   
            value     = scan_last;              //按鍵放開才回傳
            scan_last = 0xff;
         }   
      }                           
            
   }          
   
   return value;
}

void process_keyin(uchar value)            //執行按鍵相對應動作
{
   if(value < 8)
   {
   	  key = value;
      if(value == 1)
         setPixelColor(0,255, 0, 0);   	
      else if(value == 2)   
         setPixelColor(0,0, 255, 0);   	
      else if(value == 3)   
         setPixelColor(0,0, 0, 255);   	
      else if(value == 4)   
         setPixelColor(0,255, 255, 0);   	
      else if(value == 5)   
         setPixelColor(0,255, 0, 255);   	
      else if(value == 6)   
         setPixelColor(0,0, 255, 255);   	
      else if(value == 7)   
         setPixelColor(0,255, 255, 255); 
         		
      process_show(); 
      DISP_Chr(0xCF, 0x30 | value);             
   }  
   else if(value == 8)
   {
   	  disp_mode = 0;
   	  DISP_Str(0x80,"                ");   
   	  DISP_Str(0xC0,"                ");    
   	  
      DISP_Chr(0xCF, 0x30 | key);   	    	  
   }	     
   else if(value == 9)
   {
   	  disp_mode = 1;
   	  DISP_Str(0x80,"                ");   
   	  DISP_Str(0xC0,"                ");
   	  
      DISP_Chr(0xCF, 0x30 | key);   	        	     	  
   }	     
}
 
void process_show_station(void)            //顯示站別
{
	 unsigned char i;
   DISP_Str(0xC0,"Station ");
   i = station_number / 100;
   DISP_Chr(0xC8, 0x30 | i);   
   i = station_number % 100;               
   DISP_Chr(0xC9, 0x30 | (i/10));               
   DISP_Chr(0xCA, 0x30 | (i%10));    
}

void setPixelColor( uchar number, uchar color_r, uchar color_g, uchar color_b)
{
   uchar *p;  
   if(number < LED_NUM) 
   {
      p = &pixels[number * 3];
      p[0] = color_r;
      p[1] = color_g;
      p[2] = color_b;
   }
}

void process_show(void)
{
   volatile uint16_t
   loop_cnt = LED_NUM * 3; // Loop counter = 8(led number) * 3
   volatile uint8_t
   *ptr = pixels,   // Pointer to next byte
   b_data   = *ptr++,    // Current byte value
   hi,              // PORT w/output bit set high
   lo;              // PORT w/output bit set low
   volatile uint8_t next;

   cli();
   hi   = PORTD |  0x20;
   lo   = PORTD & ~0x20;   
   
   next = lo;
   if(b_data & 0x80) next = hi;
                        
   asm volatile               //1T ~= 83.3ns 
   (                          // 21 instruction clocks per bit: HHHHxxxxxxxxxxxxLLLLL
    "headD:"                   "\n\t" //        (T =  0)
     "out   %[port], %[hi]"    "\n\t" //        (T =  1)
     "rcall bitTimeD"          "\n\t" // 3      Bit 7  (T = 15)
     "out   %[port], %[hi]"    "\n\t"
     "rcall bitTimeD"          "\n\t" // Bit 6
     "out   %[port], %[hi]"    "\n\t"
     "rcall bitTimeD"          "\n\t" // Bit 5
     "out   %[port], %[hi]"    "\n\t"
     "rcall bitTimeD"          "\n\t" // Bit 4
     "out   %[port], %[hi]"    "\n\t"
     "rcall bitTimeD"          "\n\t" // Bit 3
     "out   %[port], %[hi]"    "\n\t"
     "rcall bitTimeD"          "\n\t" // Bit 2
     "out   %[port], %[hi]"    "\n\t"
     "rcall bitTimeD"          "\n\t" // Bit 1
     // Bit 0:
     "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi    (T =  1)
     "rjmp .+0"                "\n\t" // 2    nop nop      (T =  3)
     "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++   (T =  5)
     "out  %[port] , %[next]"  "\n\t" // 1    PORT = next  (T =  6)
     
     "nop"                     "\n\t" // 1                 (T =  7)  +   
     "nop"                     "\n\t" // 1                 (T =  8)  +   
     "nop"                     "\n\t" // 1                 (T =  9)  +         
     "nop"                     "\n\t" // 1                 (T = 10)  +               
     "nop"                     "\n\t" // 1                 (T = 11)  +                     
     "nop"                     "\n\t" // 1                 (T = 12)  +     
     
     "mov  %[next] , %[lo]"    "\n\t" // 1    next = lo    (T = 13)
     "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80) (T = 14)
      "mov %[next] , %[hi]"    "\n\t" // 0-1    next = hi  (T = 15)
     "nop"                     "\n\t" // 1                 (T = 16)
     "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo    (T = 17)
     "sbiw %[count], 1"        "\n\t" // 2    i--          (T = 19)
     "brne headD"              "\n\t" // 2    if(i != 0) -> (next byte)(T = 21)
      "rjmp doneD"             "\n\t"
     "bitTimeD:"               "\n\t" //                      (T =  4)
      "out  %[port], %[next]"  "\n\t" // 1    PORT = next     (T =  5)    
      "nop"                    "\n\t" // 1                    (T =  6)    
      "nop"                    "\n\t" // 1                    (T =  7)     
      "nop"                    "\n\t" // 1                    (T =  8)           
      "nop"                    "\n\t" // 1                    (T =  9)                 
      "nop"                    "\n\t" // 1                    (T = 10)                       
      "nop"                    "\n\t" // 1                    (T = 11)                 
      "mov  %[next], %[lo]"    "\n\t" // 1    next = lo       (T = 12)    
      "rol  %[byte]"           "\n\t" // 1    b <<= 1         (T = 13)    
      "sbrc %[byte], 7"        "\n\t" // 1-2  if(b & 0x80)    (T = 14)    
      "mov %[next], %[hi]"     "\n\t" // 0-1   next = hi      (T = 15)
      "nop"                    "\n\t" // 1                    (T = 16)
      "out  %[port], %[lo]"    "\n\t" // 1    PORT = lo       (T = 17)
      "ret"                    "\n\t" // 4    nop nop nop nop (T = 18-21)
      "doneD:"                 "\n"
     : [byte]  "+r" (b_data),
       [next]  "+r" (next),
       [count] "+w" (loop_cnt)
     : [port]   "I" (_SFR_IO_ADDR(PORTD)),     	
       [ptr]    "e" (ptr),
       [hi]     "r" (hi),
       [lo]     "r" (lo)
    );
    sei();    
}

void show_logo1(void)
{
   DISP_Chr(0x80, 0);	
   DISP_Chr(0x81, 1);	           
   DISP_Chr(0xC0, 2);	      
   DISP_Chr(0xC1, 3);	            
}

void show_logo2(void)
{
   DISP_Chr(0x80, 4);	
   DISP_Chr(0x81, 5);	      	                                       
   DISP_Chr(0xC0, 6);	      
   DISP_Chr(0xC1, 7);	                                                     
}

