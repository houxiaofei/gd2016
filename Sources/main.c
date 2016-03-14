#include "includes.h"



int main(void) {
	initALL();
	mdelay(1000);
	targetspeed=120;
	DataSet();
	
	targetspeed=140;
	SET_motor(targetspeed,targetspeed);
	
	initPIT();
	
	BlueTx();
	
	for(;;){
		OLED_Test();
		//KeyJudge();
//		time1=TIME;
//		time2=TIME;
//		time3=TimeMesure();
	}
  
}



