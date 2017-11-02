
void do_1s(void);
void do_500ms(void);

void do_1s(void){
	DISP_Str(LINE1,"Hello, World!");
}

void do_500ms(void){
	if(online == 1){ 
		online = 0;
		cmd = INdata[1];
		if(cmd){
			DISP_Str(LINE1+14,"On");
			CmdOut = 1;
		}else{
			DISP_Str(LINE1+14,"  ");
		}
		if (cmd == 2 ){
			sb(1000);
			CmdOut = 1;
		}
	}
	
}