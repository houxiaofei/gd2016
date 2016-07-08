#include "includes.h"

int main(void) {
	initALL();
	//mdelay(1000);
	//DataSet();
	stop_flag=0;
	targetspeed=100;
//	tsl=105;
//	tsr=120;
//	TargetSteer=3892;

	SET_motor(targetspeed,targetspeed);
	
	initPIT();

	BlueTx();

	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			ImageCapture(A,B,C);  //采图像，A-60，B-30，C-最远
			PixelScan(); //图像处理-B
			PixelScan_A();//图像处理-A
			ErrorCalculate_A();//偏差值计算-A
			ErrorCalculate();//偏差值计算-B

//			if(ABS(error-pre_error)>10)
//			{
//				error=pre_error;  //跳变时error给历史值
//			}
			
			pre_error=error;  //传递error值
			//TrendCalculate(); //ERROR趋势计算
			//EnterJudge();  //入弯判断
			//Steer_PDSet();  //舵机PD参数
			//SteerControl(); //舵机控制
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



