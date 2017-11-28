void job_usbCmd(void);
void sendReady_Host( void );

void job_usbCmd( void ) {
	if(online == 1){ 
		online = 0;
		cmd = INdata[1];
		if (cmd == 0 ){
			DISP_Str(LINE2+14,"  ");
		}
		if (cmd == 1 ) {
			station_no = INdata[2];
			DISP_Str(LINE1+10,"ST:");
			DISP_Dec(LINE1+15,station_no);
			DISP_Str(LINE2+14,"On");
			sendReady_Host();	//do nothing
		}
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