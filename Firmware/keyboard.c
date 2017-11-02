uchar keyboard_scan(void){								//返回值=0xff(無按鍵輸入), (1-7 , * = 8, # = 9) 
	uchar value, i;

	value   = 0xff;
      
	for(i=0;i<3;i++){
		//掃描線更換        腳位更改點         
		PORTC = PORTC | 0x07;							//     0    0    0    0    0    1    1    1   0x07   
		  
		  
		//keyout  腳位設1
		if(i == 0) {
			scan_line = 0;								//pc   7    6    5    4    3    2    1    0 
			PORTC = PORTC & 0xFE;						//PC0  1    1    1    1    1    1    1    0
		} else if(i == 1){
			scan_line = 1;
			PORTC = PORTC & 0xFD;						//PC1  1    1    1    1    1    1    0    1
		} else if(i == 2){
			scan_line = 2; 
			PORTC = PORTC & 0xFB;						//PC2 1    1    1    1    1    0    1    1
		} 

		_delay_us(10);
		scan_in = PINC & 0x38;            
		//keyin  腳位設0      腳位更改點 
		if(scan_in != 0x38){          					//有按鍵動作//      	       	 
			scan_in = (~scan_in)& 0x38;				//    8   4   2   1   8   4   2   1
														//pc  7   6   5   4   3   2   1   0
			if (scan_in == 0x08)scan_last = 1;			// 3  0   0   0   0   1   0   0   0     1    4    7   
			else if (scan_in == 0x10)scan_last = 2;	// 4  0   0   0   1   0   0   0   0     2    5    * 
			else if (scan_in == 0x20)scan_last = 3;	// 5  0   0   1   0   0   0   0   0     3    6    #
			else if (scan_in == 0x18)scan_last = 4;	//3+4 0   0   0   1   1   0   0   0    12   45   7*
			else if (scan_in == 0x30)scan_last = 5;	//4+5 0   0   1   1   0   0   0   0    23   56   *#
			else if (scan_in == 0x28)scan_last = 6;	//3+5 0   0   1   0   1   0   0   0    13   46   7#
														//scan_in 0   0   1   1   1   0   0   0    0x38
			scan_last = scan_last + (scan_line * 6);   
			return 0xff;                     
		}else{  
			if(i>=2){   
				value     = scan_last;					//按鍵放開才回傳
				scan_last = 0xff;
			}   
		}                               
	}          
   
	return value;
}

void process_keyin(uchar value){						//執行按鍵相對應動作
	key = value;
	trans_number();
	if(value == 1){	
		rgbled(128,0,0);   	
		lb(200);
		DISP_Str(0xca,"1");
	}else if(value == 2){	
		rgbled(0,128,0);
		lb(250);
		DISP_Str(0xca,"2");
    }else if(value == 3){
		rgbled(0,0,128); 
		lb(300); 
		DISP_Str(0xca,"3");      
	}else if(value == 7){	
		rgbled(128,128,0);  	
		lb(350);
		DISP_Str(0xca,"4");
	}else if(value == 8){      	   
		rgbled(128,0,128) ;  	
		lb(400); 
		DISP_Str(0xca,"5");            
	}else if(value == 9){      	
		rgbled(0,128,128); 	
		lb(450);    
		DISP_Str(0xca,"6");
	}else if(value == 13){	      	
		rgbled(128,128,128);
		lb(500);
		DISP_Str(0xca,"7");
	}else if(value == 14){
		rgbled(128,64,0);
		lb(550);
		DISP_Str(0xca,"*");
	}else if(value == 15){
		rgbled(32,0,32);
		lb(600);
		DISP_Str(0xca,"#");
	}else if(value == 17){
		lb(1000);
	}    
}
 