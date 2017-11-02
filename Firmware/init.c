void System_init(void){
	unsigned char wait;	
	wdt_enable(WDTO_1S);
	/* Even if you don't use the watchdog, turn it off here. On newer devices,
	 * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!	 */	 
	/* IO SETUP */   	//1=Output 0=Input
	DDRB = 0x38;    //
	PORTB = 0x00;   //
	
	#ifdef Release
		DDRC = 0x7F;    // 0111-1111
		PORTC = 0x7F;   //
	#else
		DDRC = 0x3F;    // 0011-1111
		PORTC = 0x3F;   //
	#endif
	
	DDRD = 0xEB;    //0000-1011
	PORTD = 0xF3;   //		
	
	UCSRA = 0x22;   // For more information, Please refer ATmega8A datasheet
	//TCCR1B = 0x06;
		
	usbInit();
	usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
	wait = 0;
	while(--wait){              /* fake USB disconnect for > 250 ms */            
		wdt_reset();
		_delay_ms(1);
	}
	usbDeviceConnect();
	sei();
	Init_LCD();                        /* LCD初始設定 */ 
	init_timer();
	//USARTInit(12);    //USART Initialization 115200bps UBRR = 12 http://wormfood.net/avrbaudcalc.php
	/* 變數初始設定 */ 
	
	lcdclear();
		 
	for(i=0;i<10;i++) timer[i] = 0;     
}