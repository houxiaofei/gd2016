#include "includes.h"

void Direction_Control(void);
void ModeGo(void);
void ModeBlueImage(void);
void ModeBlueSpeed(void);
void ModeOpenGo(void);

int main(void) {
	initALL();
	ChooseMode();
	if(mode>=16) ModeGo();	//跑车
	else if(mode>=8)   ModeBlueSpeed(); //看速度图像，mode=8=单环位置式，mode=8+1=双环位置式,mode=8+2=2个CCD模式
	else if(mode>=4)   ModeBlueImage(); //看CCD图像
	else if(mode>=2)   ModeOpenGo();//开环跑
}

void Direction_Control(void) //转向控制函数
{
	ImageCapture(A,B,C);  //采图像，A-60，B-30，C-最远
	PixelScan_A();//图像处理-A
	PixelScan_B(); //图像处理-B
	ErrorCalculate_A();//偏差值计算-A
	ErrorCalculate();//偏差值计算-B
	if(mode!=10)
	{
		PixelScan_C();
		SpeedFlagJudge_C();
	}
	
	//pre_error=error;  //传递error值
	//TrendCalculate(); //ERROR趋势计算
	//EnterJudge();  //入弯判断
	
	Steer_PDSet();  //舵机PD参数
	SteerControl(); //舵机控制
}

void ModeGo(void)//出发
{
	//ModeGoJudge();
	basic_mode=16;
	initPIT2();
	initPIT();
	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			Direction_Control();
		}
//		time1=TIME;
//		time2=TIME; 
//		time3=TimeMesure();
	}
}

void ModeBlueSpeed(void)//看速度图像
{
	basic_mode=8;
	Image_or_not=13;//从data开始，发送error,CurrentSteer,csl,csr,tsl,tsr
	Speed_or_not=35;//发送舵机值后，接着发送速度数据
	OLED_Init();//OLED初始化
	BlueTx();//蓝牙发送开始
	initPIT2();
	initPIT();
	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			Direction_Control();
		}
		if(oled_flag==1)
		{
			oled_flag=0;
			OLED_Test();
		}
	}
}

void ModeBlueImage(void)//看CCD图像
{
	basic_mode=4;
	Image_or_not=1;//从CCD图像开始，发送图像，flag,error,a_error,b_error,CurrentSteer
	Speed_or_not=57;//发送完舵机值后结束
	OLED_Init();//OLED初始化
	BlueTx();//蓝牙发送开始
	initPIT2();
	initPIT();
	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			Direction_Control();
		}
		if(oled_flag==1)
		{
			oled_flag=0;
			OLED_Test();
		}
		KeyJudge();
	}
}
void ModeOpenGo(void)//开环跑
{
	basic_mode=2;
	targetspeed=230;
	SET_motor(100,100);
	OLED_Init();//OLED初始化
	initPIT2();
	initPIT();
	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			//Direction_Control();
		}
		if(oled_flag==1)
		{
			oled_flag=0;
			OLED_Test();
		}
		KeyJudge();
	}
}


void ModeGoJudge(void)
{
	switch(mode){
	case 18://高速，切内
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 19://高速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 20://中速，切内
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 21://中速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 24://低速，切内
		straightspeed=200,transspeed=160,turnspeed=160,deadspeed=160,barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 25://低速，切中
		straightspeed=200,transspeed=165,turnspeed=165,deadspeed=165,barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 26://低速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 27://低速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 28://低速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 29://低速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 30://低速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 31://低速，切中
		straightspeed=230,transspeed=190,turnspeed=190,deadspeed=190,barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	}
}

