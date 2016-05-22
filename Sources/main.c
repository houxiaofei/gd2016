#include "includes.h"

int main(void) {
	initALL();
	//mdelay(1000);
	//DataSet();
	stop_flag=0;
	
	targetspeed=150;
//	tsl=105;
//	tsr=120;
//	TargetSteer=3892;


	//SET_motor(targetspeed,targetspeed);
	initPIT();
	
	BlueTx();
	
	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			ImageCapture(A,B);
			PixelScan();
			PixelScan_A();
			ErrorCalculate_A();
			ErrorCalculate();
			Steer_PDSet();
			SteerControl();
		}
		if(oled_flag==1)
		{
			oled_flag=0;
			OLED_Test();
		}
		KeyJudge();
		//OLED_Test();
//		time1=TIME;
//		time2=TIME; 
//		time3=TimeMesure();
	}
  
}



