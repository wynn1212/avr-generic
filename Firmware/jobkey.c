void job_keypad(char ch );

void job_keypad(char ch ){
	DISP_Chr(LINE2,ch);
	if(ch == '/') B_buzzon = !B_buzzon;
}
