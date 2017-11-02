char keypadhit(void);
void putcharJ( unsigned char  );
uchar KeyConvert( char r, char c );

uchar GetKeyPressed( void ){    
	static char pressed = 0, get2key = 2;
	uchar r, ch, /*ch_shift,*/ rc = 0;
	//DISP_Dec(LINE2,pressed);
	if ( !pressed ){
		for (r=0; r<4; r++) {
			digitalWrite4(~(1<<r));
			_delay_us( 5 );	//等硬體反應Tr
			ch = digitalRead3() & 0x07;
			if( ch != 0x07) {
				//ch_shift = (ch>>5); // right shift xxx0-0000 to 0000-0xxx
				rc = KeyConvert( r, ch );
				if ( (ch =='*') || (ch == '#') ){
					if (--get2key) ch =0;
				} else {
					get2key = 2;
				}
				pressed = 1;
			}
		}
	} else {

		digitalWrite4(0);
		_delay_us( 30 );	//等硬體反應Tr
		ch = digitalRead3() & 0x07;	//
		
		if ( ch == 0x07 ) pressed = 0;
	}
    return rc;//Indicate No key pressed
}

char keypadhit(void){
	return GetKeyPressed();
}
uchar KeyConvert( char r, char c ) {
	uchar val = 0;
	//DISP_Hex(LINE1,c);
	//DISP_Hex(LINE2,r);
	if (c == 0x06 && r == 0x00) val='1';
	if (c == 0x05 && r == 0x00) val='2';
	if (c == 0x03 && r == 0x00) val='3';
	if (c == 0x04 && r == 0x00) val='a';
	if (c == 0x02 && r == 0x00) val='b';
	if (c == 0x06 && r == 0x01) val='4';
	if (c == 0x05 && r == 0x01) val='5';
	if (c == 0x03 && r == 0x01) val='6';
	if (c == 0x04 && r == 0x01) val='c';
	if (c == 0x02 && r == 0x01) val='d';
	if (c == 0x06 && r == 0x02) val='7';
	if (c == 0x05 && r == 0x02) val='8';
	if (c == 0x03 && r == 0x02) val='9';
	if (c == 0x04 && r == 0x02) val='e';
	if (c == 0x02 && r == 0x02) val='f';
	if (c == 0x06 && r == 0x03) val='*';
	if (c == 0x05 && r == 0x03) val='0';
	if (c == 0x03 && r == 0x03) val='#';
	if (c == 0x04 && r == 0x03) val='g';
	if (c == 0x02 && r == 0x03) val='/';
	return val;
}
