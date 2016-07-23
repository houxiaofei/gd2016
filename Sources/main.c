#include "includes.h"

void Direction_Control(void);
void ModeGo(void);
void ModeBlueImage(void);;
void ModeOpenGo(void);
void ModeGoJudge(void);
void ModeSteerModify(void);
void ModeGoDebug(void);
void ModeGoJudgeDebug(void);

unsigned long OLED_cnt;

    int main(void) {
	initALL();
	OLED_Init();
	ChooseMode();
	if(switch5==1)
	{
		ModeGoJudge();
		while(switch1){
			OLED_cnt++;
			KeyJudge();
			if(OLED_cnt>=10000)
			{
				OLED_cnt=0;
				OledGo();
			}
		}
		ChooseMode();
		if(mode>=16) ModeGo();	//跑车
	}
	else
	{
		if(mode>=8)   ModeGoDebug(); //跑车
		else if(mode==5)   ModeBlueImage(); //看CCD图像
		else if(mode==3)   ModeOpenGo();//开环跑
		else if(mode==1) ModeSteerModify();//调舵机中值
	}
}

void Direction_Control(void) //转向控制函数
{
	ImageCapture(A,B,C);  //采图像，A-60，B-30，C-最远
	PixelScan_A();//图像处理-A
	PixelScan_B(); //图像处理-B
	PixelScan_C(); //图像处理-C
	ErrorCalculate_A();//偏差值计算-A
	ErrorCalculate();//偏差值计算-B
	Steer_PDSet();  //舵机PD参数
	SteerControl(); //舵机控制
}

void ModeGoDebug(void)//调试模式跑
{
	ModeGoJudgeDebug();
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


void ModeGo(void)//出发
{
	basic_mode=16;
	initPIT();
	while(!start_flag){}
	initPIT2();
	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			Direction_Control();
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
	targetspeed=190;
	straightspeed=190;
	turnspeed=150;
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


void ModeGoJudgeDebug(void)
{
	switch(mode){
	case 25://第一圈
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 21://切外，第二圈
		straightspeed=230;turnspeed=170;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 23://障碍物不过，第二圈
		straightspeed=230;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 19://第三圈
		straightspeed=210;turnspeed=160;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 27://第一圈,50
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		steer_amb_value=50;
		break;
	case 29://第二圈，200，170
		straightspeed=200;turnspeed=170;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 31://第一圈,200
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		steer_amb_value=200;
		break;
	case 9:
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		steer_amb_value=300;
		break;
	case 11:
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		barleft_kp=4;barright_kp=2;
		break;
	case 13:
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		barleft_kp=6;barright_kp=3.5;
		a_edge_diff=130;b_edge_diff=130;
		break;
	case 15:
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		barleft_kp=6;barright_kp=3.5;
		b_value_end=50;
		break;
	}
}
void ModeGoJudge(void)
{
	switch(mode){
	case 8://第一圈
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 4://切外，第二圈
		straightspeed=230;turnspeed=170;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 6://障碍物不过，第二圈
		straightspeed=230;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 2://第三圈
		straightspeed=210;turnspeed=160;
		Speed_kc1=13000;sp_x1=0.015;sp_x2=4;sp_x3=35;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	}
}

