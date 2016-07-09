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
	else if(mode>=8)   ModeBlueImage(); //看CCD图像
	else if(mode>=4)   ModeBlueSpeed(); //看速度图像
	else if(mode>=2)   ModeOpenGo();//开环跑
}

void Direction_Control(void) //转向控制函数
{
	ImageCapture(A,B,C);  //采图像，A-60，B-30，C-最远
	PixelScan_B(); //图像处理-B
	PixelScan_A();//图像处理-A
	ErrorCalculate_A();//偏差值计算-A
	ErrorCalculate();//偏差值计算-B
	
	//pre_error=error;  //传递error值
	//TrendCalculate(); //ERROR趋势计算
	//EnterJudge();  //入弯判断
	
	Steer_PDSet();  //舵机PD参数
	SteerControl(); //舵机控制
}

void ModeGo(void)//出发
{
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

void ModeBlueImage(void)//看CCD图像
{
	Image_or_not=1;//从CCD图像开始，发送图像，flag,error,a_error,b_error,CurrentSteer
	Speed_or_not=52;//发送完舵机值后结束
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
void ModeBlueSpeed(void)//看速度图像
{
	Image_or_not=30;//从舵机值开始，发送CurrentSteer,csl,csr,tsl,tsr
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
void ModeOpenGo(void)//开环跑
{
	targetspeed=100;
	SET_motor(targetspeed,targetspeed);
	initPIT();
	for(;;){
		if(steer_flag==1)
		{
			steer_flag=0;
			Direction_Control();
		}
	}
	KeyJudge();
}

