typedef unsigned char uint8_t;
extern char strE[32];

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address and data registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}
//
void eeprom_string_write(char *str, int record ){
	if (record > 15){
//		printi("E: reach max limit(15)\n\r",0);
	}else{
		uchar i = 0;
		uchar addr = 0;
		char ByteOfData;
		record = record*32;
		do {
			ByteOfData = strE[addr];
			EEPROM_write( (unsigned int )record++, ByteOfData);
		}
		while ( str[addr++] && i<32 );
	}
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}

void eeprom_string_read(int record){
	uchar i = 0, addr = 0;
	char ByteOfData;
	if (record > 15){
	}else{
		addr = record*32;
		do {
			ByteOfData = EEPROM_read(addr);
			strE[addr] = ByteOfData;
		}
		while ( strE[addr++] && i<32 );		
	}
}