#include "includes.h"



int main(void) {
	initALL();
	//mdelay(1000);
	//DataSet();
	stop_flag=0;
	
	targetspeed=230;

	SET_motor(targetspeed,targetspeed);
	initPIT();
	
	BlueTx();
	
	for(;;){
		KeyJudge();
		//OLED_Test();
//		time1=TIME;
//		time2=TIME;
//		time3=TimeMesure();
	}
  
}



