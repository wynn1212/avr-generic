/*
	This example should run on most AVRs with only little changes. No special
	hardware resources except INT0 are used. You may have to change usbconfig.h for
	different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
	at least be connected to INT0 as well.
*/

/*TODO Lists:
	Fix alarm no-input show input-error glitch(When EEPROM has data)
	Include EEPROM data on toggle alarm
	Improve 2s-maskable interrupt
	fix during "save ok" session press '*' and '1'~'4' during 2s glitch
*/
#include "main.h"
#include "iopin.c"
#include "lcd.c" 	
#include "usb.c"
#include "beep.c"
#include "eeprom.c"
#include "init.c"	
#include "keypad.c"
#include "jobkey.c"
#include "jobtimer.c"


/* ------------------------------------------------------------------------- */
ISR (TIMER0_OVF_vect){		// timer0 overflow interrupt
	uchar   i; 
	
	TCNT0 = 22;  			// event to be exicuted every 5ms here
	for(i=0;i<10;i++){
		if(timer[i]>0)timer[i]--;           
	}    
}

ISR(INT1_vect){   }
//
ISR (TIMER1_OVF_vect){						// timer0 overflow interrupt
	TCNT1 = t1cnt;
	if(B_buzzon)digitalWrite(BEEP,0xff);		//PD1 接 BEEP   腳位更改點 
}
 
int main(void){	
	uchar ch;

	System_init();	
	lcdclear();
	for(;;){          						/* main event loop */   
		wdt_reset();
		usbPoll();
		if(timer[0] ==0){					//5ms x 6 = 30ms, 鍵盤掃描時間
			timer[0] = 6;
			//usbPoll();
		}
		if(timer[1] ==0){					//5ms x 200 = 1s 上數
			timer[1] = 200; 				//200 for normal, 20 for test
			job_1s();			
		}  		
		if(timer[2] ==0){					//5ms x 100 = 0.5s 上數 
			timer[2] = 100; 
			job_500ms();
			
		}
		if(timer[3] == 0) {
			timer[3] = 20;	//100mS			
			ch = GetKeyPressed();  //********************************************************
			digitalWrite(19,0xff);	//19 Blink Run LED			
			if ( ch ) job_keypad( ch );
			
		}
		if(B_wait200ms){
			timer[4] = 40;
			B_wait200ms = 0;
			B_wait200msreset = 1;
		}
		if(timer[4] == 0 && B_wait200msreset){
			do_timer200ms();
			B_wait200msreset = 0;
		}
	}
	return 0;
}

void job_usbCmd( void ) {
	if(online == 1){ 
		online = 0;
		cmd = INdata[1];
		
		if (cmd == 1 ) sendReady_Host();	//do nothing
		if (cmd == 2 ) {
			lb(1000);
			B_wait200ms = 1;
		}
		//if (cmd == 3 ) sendEE_Host( INdata[2] ) ; // INdata[2] = Address
		//if (cmd == 4 ) recvEE_Host( INdata[2] ) ;
	}
}
void sendReady_Host( void ){	
	unsigned char i;
	for ( i=0;i<8;i++) OUTdata[i] = 0; //
	OUTdata[0] = 1;		//CmdOut = 1;
	usbPoll();	//excute
}
/* ------------------------------------------------------------------------- */
void init_timer(void){
	//timer0
	TIMSK |= (1 << TOIE0);					// TIMER0設定
	TCCR0 |= (1 << CS02);					// set prescaler to 256 and start the timer 
	TCNT0 = 22; 	
	//timer1
	TCCR1B |= (1 << CS10);					// set prescaler to No prescaling
	TCNT1 = 0;
}

void vbmode(void){
}

void job_1s(void){
	do_1s();
}

void job_500ms(void){
	do_500ms();
}
