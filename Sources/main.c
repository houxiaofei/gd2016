#include "includes.h"

void Direction_Control(void);
void ModeGo(void);
void ModeBlueImage(void);
void ModeBlueSpeed(void);
void ModeOpenGo(void);
void ModeGoJudge(void);
void ModeSteerModify(void);

unsigned long OLED_cnt;

int main(void) {
	initALL();
	OLED_Init();
//	ChooseMode();
//	ModeGoJudge();
//	while(switch1){
//		OLED_cnt++;
//		KeyJudge();
//		if(OLED_cnt>=10000)
//		{
//			OLED_cnt=0;
//			OledGo();
//		}
//	}
	ChooseMode();
	if(mode>=16) ModeGo();	//跑车
	else if(mode>=8)   ModeBlueSpeed(); //看速度图像，mode=8=单环位置式，mode=8+1=双环位置式,mode=8+2=2个CCD模式
	else if(mode>=4)   ModeBlueImage(); //看CCD图像
	else if(mode>=2)   ModeOpenGo();//开环跑
	else ModeSteerModify();//调舵机中值
}

void Direction_Control(void) //转向控制函数
{
	ImageCapture(A,B,C);  //采图像，A-60，B-30，C-最远
	PixelScan_A();//图像处理-A
	PixelScan_B(); //图像处理-B
	ErrorCalculate_A();//偏差值计算-A
	ErrorCalculate();//偏差值计算-B
	Steer_PDSet();  //舵机PD参数
	SteerControl(); //舵机控制
}

void ModeGo(void)//出发
{
	ModeGoJudge();
	basic_mode=16;
	if(mode==31)
	{
		targetspeed=straightspeed;
		SET_motor(targetspeed,targetspeed);
		OLED_Init();//OLED初始化
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
	else
	{
		initPIT();
		OLED_Init();
		while(!start_flag){}
		initPIT2();
		for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			Direction_Control();
		}
		if(oled_flag==1)
		{
			oled_flag=0;
			OledGo();
		}
		KeyJudge();
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
	SET_motor(targetspeed,targetspeed);
	OLED_Init();//OLED初始化
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

void ModeSteerModify(void)
{
	basic_mode=1;
	targetspeed=100;
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
		straightspeed=265;transspeed=190;turnspeed=190;deadspeed=210;barspeed=190;
		Speed_kc1=10000;sp_x1=0.009;sp_x2=4.5;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		barleft_kp=4.5,barright_kp=3.5;
		break;
	case 19://高速，切中
		straightspeed=265;transspeed=190;turnspeed=190;deadspeed=210;barspeed=190;
		Speed_kc1=10000;sp_x1=0.0095;sp_x2=3.5;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		barleft_kp=4.5,barright_kp=3.5;
		break;
	case 20://中速，切内
		straightspeed=230;transspeed=180;turnspeed=180;deadspeed=180;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=3;sp_x3=30;Speed_kc1a=13000;Speed_kc1b=10000;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;Motor_PWM_MAX=400;
		break;
	case 21://中速，切中
		straightspeed=245;transspeed=180;turnspeed=180;deadspeed=190;barspeed=180;
		//Speed_kc1=11000;sp_x1=0.0075;sp_x2=3.5;sp_x3=30;//error>0
		Speed_kc1=13000;sp_x1=0.006;sp_x2=3;sp_x3=30;Speed_kc1a=15000;Speed_kc1b=13000;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;Motor_PWM_MAX=400;
		break;
	case 24://低速，切内
		straightspeed=200;transspeed=160;turnspeed=160;deadspeed=160;barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;Motor_PWM_MAX=400;
		break;
	case 25://低速，切中
		straightspeed=200;transspeed=165;turnspeed=165;deadspeed=165;barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;Motor_PWM_MAX=400;
		break;
	case 26://高高速，切内
		straightspeed=255;transspeed=215;turnspeed=215;deadspeed=230;barspeed=190;
		Speed_kc1=10000;sp_x1=0.0095;sp_x2=4;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 27://高高速，切中
		straightspeed=255;transspeed=215;turnspeed=215;deadspeed=230;barspeed=190;
		Speed_kc1=10000;sp_x1=0.0095;sp_x2=3.5;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 28://低低速，切内
		straightspeed=190;transspeed=150;turnspeed=150;deadspeed=160;barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;Motor_PWM_MAX=400;
		break;
	case 29://低低速，切中
		straightspeed=190;transspeed=155;turnspeed=155;deadspeed=165;barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;Motor_PWM_MAX=400;
		break;
	case 30://低速，切中
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 31://开环
		straightspeed=190;transspeed=150;turnspeed=150;deadspeed=170;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	}
}
//void ModeGoJudge(void)
//{
//	switch(mode){
//	case 2://高速，切内
//		straightspeed=265;transspeed=190;turnspeed=190;deadspeed=210;barspeed=190;
//		Speed_kc1=10000;sp_x1=0.009;sp_x2=4.5;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
//		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		barleft_kp=4.5,barright_kp=3.5;
//		break;
//	case 3://高速，切中
//		straightspeed=265;transspeed=190;turnspeed=190;deadspeed=210;barspeed=190;
//		Speed_kc1=10000;sp_x1=0.0095;sp_x2=3.5;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
//		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		barleft_kp=4.5,barright_kp=3.5;
//		break;
//	case 4://中速，切内
//		straightspeed=230;transspeed=180;turnspeed=180;deadspeed=180;barspeed=180;
//		Speed_kc1=10000;sp_x1=0.006;sp_x2=3;sp_x3=30;Speed_kc1a=13000;Speed_kc1b=10000;
//		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 5://中速，切中
//		straightspeed=245;transspeed=180;turnspeed=180;deadspeed=190;barspeed=180;
//		//Speed_kc1=11000;sp_x1=0.0075;sp_x2=3.5;sp_x3=30;//error>0
//		Speed_kc1=13000;sp_x1=0.006;sp_x2=3;sp_x3=30;Speed_kc1a=15000;Speed_kc1b=13000;
//		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 8://低速，切内
//		straightspeed=200;transspeed=160;turnspeed=160;deadspeed=160;barspeed=170;
//		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
//		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 9://低速，切中
//		straightspeed=200;transspeed=165;turnspeed=165;deadspeed=165;barspeed=170;
//		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
//		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 10://高高速，切内
//		straightspeed=250;transspeed=215;turnspeed=220;deadspeed=240;barspeed=190;
//		Speed_kc1=10000;sp_x1=0.0095;sp_x2=4.5;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
//		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 11://高高速，切中
//		straightspeed=250;transspeed=215;turnspeed=215;deadspeed=230;barspeed=190;
//		Speed_kc1=10000;sp_x1=0.0095;sp_x2=3.5;sp_x3=30;Speed_kc1a=12000;Speed_kc1b=10000;
//		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 12://低速，切中
//		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
//		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
//		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 13://低速，切中
//		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
//		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
//		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 14://低速，切中
//		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
//		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
//		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	case 15://开环
//		straightspeed=190;transspeed=150;turnspeed=150;deadspeed=170;barspeed=180;
//		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
//		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
//		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
//		break;
//	}
//}

