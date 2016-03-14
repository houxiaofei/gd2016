#include "includes.h"



int main(void) {
	initALL();
	mdelay(1000);
	//DataSet();
	targetspeed=120;
	SET_motor(targetspeed,targetspeed);
	
	initPIT();
	
	BlueTx();
	
	for(;;){
		KeyJudge();
		OLED_Test();
	}
  
}



