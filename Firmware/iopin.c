/*
	I/O PIN Direction			_______________
	(1)[PIN22]PC6(!RESET)------|O	   U	   |--(SCL/ADC5)PC5[PIN21](28)
	(2)[PIN0]PD0(RxD)----------|			   |--(SDA/ADC4)PC4[PIN20](27)
	(3)[PIN1]PD1(TxD)----------|			   |------(ADC3)PC3[PIN19](26)
	(4)[PIN2]PD2(INT0)---------|			   |------(ADC2)PC2[PIN18](25)
	(5)[PIN3]PD3(INT1)---------|			   |------(ADC1)PC1[PIN17](24)
	(6)[PIN4]PD4(XCK/T0)-------|			   |------(ADC0)PC0[PIN16](23)
	(7)VCC---------------------|   Atmel AVR   |------------------AGND(22)
	(8)GND---------------------|  ATMEGA8A-PU  |------------------AREF(21)
	(9)[PIN14]PB6(XT1/TOSC1)---|			   |------------------AVCC(20)
	(10)[PIN15]PB7(XT2/TOSC2)--|			   |-------(SCK)PB5[PIN13](19)
	(11)[PIN5]PD5(T1)----------|			   |------(MISO)PB4[PIN12](18)
	(12)[PIN6]PD6(AIN0)--------|			   |--(OC2/MOSI)PB3[PIN11](17)
	(13)[PIN7]PD7(AIN1)--------|		  	   |---(SS/OC1B)PB2[PIN10](16)
	(14)[PIN8]PB0(ICP)---------|_______________|-------(OC1A)PB1[PIN9](15)
	Note: return 0 if LOW, 1 if HIGH 
*/

void digitalWrite( unsigned char, unsigned char );	//pin no, val=0,1,0xFF
void pinMode(unsigned char, unsigned char );			//pin no, 0=input, 1=output
unsigned char digitalRead(unsigned char pin);
unsigned char digitalRead4(void);

//return 0 if LOW, 1 if HIGH
unsigned char digitalRead(unsigned char pin) {	
	char rc = 1 << (pin % 8);	//0-7
	if (pin < 8) rc = PIND & rc; 
	if ((pin > 7) && (pin < 16)) rc = PINB & rc;
	if (pin > 15) rc = PINC & rc;
	if ( rc ) rc = 1;
	return rc;
}

//val=0 for input, 1 for output
void pinMode(unsigned char pin , unsigned char val ) {
	char ch = 1 << (pin % 8);	//0-7
	if ( !val ) {	// clear bit
		if (pin < 8) DDRD &= ch; 
		if ((pin > 7) && (pin < 16)) DDRB &= ch;
		if (pin > 15) DDRC &= ch;
	} else {
		if (pin < 8) DDRD |= ch;
		if ((pin > 7) && (pin < 16)) DDRB |= ch;
		if (pin > 15) DDRC |= ch;
	}
}

// 0=clear bit 0xFF=XOR, 1~254=set bit
void digitalWrite(unsigned char pin, unsigned char val ){
	char ch = 1 << (pin % 8);	//0-7
	if ( !val ) {	// clear bit
		ch = ~ch;
		if (pin < 8) PORTD &= ch; 
		if ((pin > 7) && (pin < 16)) PORTB &= ch;
		if (pin > 15) PORTC &= ch;
	} else {
		if ( val == 0xFF ) {	//XOR
			if (pin < 8) PORTD ^= ch;
			if ((pin > 7) && (pin < 16)) PORTB ^= ch;
			if (pin > 15) PORTC ^= ch;
		} else {
			if (pin < 8) PORTD |= ch;
			if ((pin > 7) && (pin < 16)) PORTB |= ch;
			if (pin > 15) PORTC |= ch;
		} 
	}
}

unsigned char bytepin[8] = {3,1,0,21,20,19,18,17};	//LCD
void digitalWrite8(unsigned char val) {
	unsigned char i,ch;
	for(i=0;i<8;i++){
		ch = val & (1<<i);
		if (ch){
			digitalWrite(bytepin[i],1);
		} else {
			digitalWrite(bytepin[i],0);
		}
	}
}
unsigned char digitpin[4] = {16,13,12,11};	//Keypad
void digitalWrite4( unsigned char val) {
	unsigned char i,ch;
	for(i=0;i<4;i++){
		ch = val & (1<<i);
		if (ch){
			digitalWrite(digitpin[i],1);
		} else {
			digitalWrite(digitpin[i],0);
		}
	}
}

unsigned char readpin[3] = {10,9,8}; //Keypad Read
unsigned char digitalRead3(void){
	unsigned char i,ch=0;
	for(i=0;i<3;i++){
		ch = ch | (digitalRead(readpin[i])<<i);
	}
	return ch;
}