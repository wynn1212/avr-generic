void lb(int f){
	t1cnt = 65536-(6000000/f);                              
	TIMSK |= (1 << TOIE1); 
}
void sb(int f){
	t1cnt = 65536-(6000000/f);                              
	TIMSK |= (1 << TOIE1); 
	_delay_ms(100);
	//PORTC = PORTC & 0xBF;                              //BEEP關閉   腳位更改點 
	TIMSK &= ~(1 << TOIE1); 
}
void cb(void){
	//PORTC = PORTC & 0xBF;                              //BEEP關閉   腳位更改點 
	TIMSK &= ~(1 << TOIE1);    
}
