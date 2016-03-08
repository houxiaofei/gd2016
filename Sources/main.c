#include "includes.h"



int main(void) {
	initALL();
	mdelay(1000);
	//DataSet();
	
	OLED_Test();
	
	targetspeed=120;
	SET_motor(targetspeed,targetspeed);
	
	initPIT();
	
	BlueTx();
	
	for(;;){
		KeyJudge();
	}
  
}



