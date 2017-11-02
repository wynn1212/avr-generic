//LCD�Ҳը禡�A�Ω��r��LCD
//lcd.h
#include <util/delay.h> 					/* for _delay_ms() */

/*   =======  �w�鱵�}���w�q  =============  */

void DISP_LCDBuff(char *str);
void LCD_E( char b );
void LCD_RW( char b );
void LCD_RS( char b );
void LCD_CMD(char ch);
void LCD_COM(char cmd );
const uchar picture1[2][8] ={	{0x11,0x0E,0x0E,0x0E,0x0E,0x0E,0x11,0x1F},
								{0x1F,0x1F,0x09,0x06,0x0E,0x0E,0x0E,0x1F}, 
                                } ;

const uchar picture2[3][8] ={	{0x11,0x0E,0x0E,0x0E,0x0E,0x0E,0x11,0x1F},                                                            
								{0x19,0x16,0x17,0x03,0x17,0x17,0x17,0x1F},                              
								{0x19,0x16,0x17,0x03,0x17,0x17,0x17,0x1F},                                                                                                                                                                                                                                                                                                                                
								} ;

void Init_LCD( void ){						// LCD ��l�ƨ禡
	char ch = 0x38;		//8BIT
	cli();
	//if ( BITLEN==4 ) ch = 0x28;
	_delay_ms(dT);						// ���� LCD �q���}�ҧ���
	LCD_CMD(ch);		//
	_delay_ms(dT);
	LCD_CMD(ch);		// 
	_delay_ms(dT);
	LCD_CMD(ch);		// 
	_delay_ms(dT);
	LCD_CMD(0x08);
	_delay_ms(dT);
	LCD_CMD(0x01);
	_delay_ms(dT);
	LCD_CMD(0x06);
	_delay_ms(dT);
	LCD_CMD(0x0C);
	_delay_ms(dT);
	LCD_CMD(0x80);
	_delay_ms(dT);
	sei();
	//init_LCD_CGRAM();
} 

uchar LCD_BUSY(void){						/* 0 for Ready */
	_delay_us(dT);                    		// �@�ӵu�ɶ�������ɧ�   
	return 0;  
}
void LCD_CMD(char ch){						// �g�J���O�Ȧs���禡
	LCD_RS(0);								//RS = 	0
	LCD_COM( ch );
}

void LCD_DATA(char ch) {					// �g�J��ƼȦs���禡
	LCD_RS(1);								//RS = 1
	LCD_COM( ch );
}
void LCD_COM(char cmd ) {	
	//LCD_RW(0);								//RW = 0
    while (LCD_BUSY()); 
	digitalWrite8(cmd);	
	_delay_us( dT );						// �@�ӵu�ɶ�������ɧ�	
	LCD_E(1);								//E  = 1		
	_delay_us( dT );						// �@�ӵu�ɶ�������ɧ�	
	LCD_E(0);								//E  = 0	
}
void DISP_Str(char addr1,char *str) {		// �bLCD���w��m��ܦr��禡
	LCD_CMD(addr1);							// �]�wDD RAM��}�Ĥ@��Ĥ@�C
	while(*str !=0) LCD_DATA(*str++);		// �I�s��ܦr��禡
}

void DISP_Chr(char col,char chrx) {			// �bLCD���w��m��ܦr���禡 
	LCD_CMD(col);         
	LCD_DATA(chrx);	                    
}

void DISP_Dec(char col,char bytex) {		// �bLCD���w��m��ܦr���禡 
	LCD_CMD(col);         
	LCD_DATA((bytex%10)+'0');				//'0' = 0x30
	if (bytex > 9){
		LCD_CMD(--col);
		LCD_DATA(((bytex/10)%10)+0x30);	
	}
	if (bytex > 99){
		LCD_CMD(--col);
		LCD_DATA(((bytex/100)%10)+0x30);	
	}
}

void DISP_Int(char addr1,long v1) {			// �bLCD���w��m��ܦr���禡  
	char i;
	for (i=0;i<7;i++){
		LCD_CMD(addr1-i+7);
		LCD_DATA((v1%10)+0x30);
		v1/=10;
		if(v1==0)
		break;
	}
}

void DISP_Hex(char addr1,unsigned char v1) {// �bLCD���w��m��ܦr���禡
	unsigned char i,j;
	i = v1 / 16 ;
	j = v1 % 16 ;
	DISP_Chr(addr1,'0');
	DISP_Chr(addr1 + 1,'x');   
	DISP_Chr(addr1 + 2 ,HexData[i]);
	DISP_Chr(addr1 + 3 ,HexData[j]);
}

void lcd_set_char1(void) {					// address = 0 -7
   uchar i,address;

    for(address=0;address<2;address++) {   
      LCD_CMD( 0x40 | (address*8) );		//set address   
      for(i=0;i<8;i++)         	
	       LCD_DATA(picture1[address][i]);	     
	}
}

void lcd_set_char2(void) {					// address = 0 -7
   uchar i,address;

    for(address=2;address<5;address++) {   
      LCD_CMD( 0x40 | (address*8) );		//set address   
      for(i=0;i<8;i++)         	
	       LCD_DATA(picture2[address-2][i]);	     
	}
}

void show_logo1(void) {
   lcd_set_char1();							/* �ۦ�w�q�r�� */		 
   
   //DISP_Chr(LINE1, 0);	
   //DISP_Chr(LINE1+1, 1);	           
}

void show_logo2(void) {
   lcd_set_char2();							/* �ۦ�w�q�r�� */
   	 
   //DISP_Chr(LINE1+3, 2);	
   //DISP_Chr(LINE1+4, 3);
   //DISP_Chr(LINE1+5, 4);
}


void lcdclear(void) {
	DISP_Str(0x80,"                ");   
	DISP_Str(0xC0,"                "); 
}

void lcdclear2(void) {   
	DISP_Str(0xC0,"                "); 
}

void lcdclear1(void) {
	DISP_Str(0x80,"                ");
}
	
void LCD_E( char b ) {      	//LCD Enabler 
	if ( b ) digitalWrite(E,1);//PORTD |= 0x08;     	//PD3 = 1 
	if ( !b ) digitalWrite(E,0);//PORTD &= ~0x08;    	//PD3 = 0 
	_delay_us(3);
	//if ( b ) PORTD |= 0x20;     	//PD5 = 1 
	//if ( !b ) PORTD &= ~0x20;    	//PD5 = 0 
} 

void LCD_RS( char b ) { 
 if ( b ) digitalWrite(RS,1);//PORTD |= 0x01;     	//PD0
 if ( !b ) digitalWrite(RS,0);//PORTD &= ~0x01;    	// 
 //if ( b ) PORTD |= 0x02;     	//
 //if ( !b ) PORTD &= ~0x02;    // 
} 

void LCD_RW( char b ) { 
	#ifdef Release
		if ( b ) digitalWrite(RW,1);//PORTD |= 0x01;     	//PD0
		if ( !b ) digitalWrite(RW,0);//PORTD &= ~0x01;    	// 
	#else
		b=0;
	#endif
	//if ( b ) PORTD |= 0x08; 
	//if ( !b ) PORTD &= ~0x08; 
}
