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
			ImageCapture(A,B,C);  //��ͼ��A-60��B-30��C-��Զ
			PixelScan(); //ͼ����-B
			PixelScan_A();//ͼ����-A
			ErrorCalculate_A();//ƫ��ֵ����-A
			ErrorCalculate();//ƫ��ֵ����-B

//			if(ABS(error-pre_error)>10)
//			{
//				error=pre_error;  //����ʱerror����ʷֵ
//			}
			
			pre_error=error;  //����errorֵ
			//TrendCalculate(); //ERROR���Ƽ���
			//EnterJudge();  //�����ж�
			//Steer_PDSet();  //���PD����
			//SteerControl(); //�������
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



