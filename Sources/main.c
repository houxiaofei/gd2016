#include "includes.h"



int main(void) {
	initALL();
	//mdelay(1000);
	//DataSet();
	
	targetspeed=150;
	SET_motor(targetspeed,targetspeed);
	
	//initPIT();
	
	BlueTx();
	
	for(;;){
		KeyJudge();
		OLED_Test();
//		time1=TIME;
//		time2=TIME;
//		time3=TimeMesure();
	}
  
}



