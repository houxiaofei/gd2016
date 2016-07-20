#include "includes.h"

void Direction_Control(void);
void ModeGo(void);
void ModeBlueImage(void);
void ModeBlueSpeed(void);
void ModeOpenGo(void);

int main(void) {
	initALL();
	ChooseMode();
	if(mode>=16) ModeGo();	//�ܳ�
	else if(mode>=8)   ModeBlueSpeed(); //���ٶ�ͼ��mode=8=����λ��ʽ��mode=8+1=˫��λ��ʽ,mode=8+2=2��CCDģʽ
	else if(mode>=4)   ModeBlueImage(); //��CCDͼ��
	else if(mode>=2)   ModeOpenGo();//������
}

void Direction_Control(void) //ת����ƺ���
{
	ImageCapture(A,B,C);  //��ͼ��A-60��B-30��C-��Զ
	PixelScan_A();//ͼ����-A
	PixelScan_B(); //ͼ����-B
	ErrorCalculate_A();//ƫ��ֵ����-A
	ErrorCalculate();//ƫ��ֵ����-B
	if(mode!=10)
	{
		PixelScan_C();
		SpeedFlagJudge_C();
	}
	
	//pre_error=error;  //����errorֵ
	//TrendCalculate(); //ERROR���Ƽ���
	//EnterJudge();  //�����ж�
	
	Steer_PDSet();  //���PD����
	SteerControl(); //�������
}

void ModeGo(void)//����
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

void ModeBlueSpeed(void)//���ٶ�ͼ��
{
	basic_mode=8;
	Image_or_not=13;//��data��ʼ������error,CurrentSteer,csl,csr,tsl,tsr
	Speed_or_not=35;//���Ͷ��ֵ�󣬽��ŷ����ٶ�����
	OLED_Init();//OLED��ʼ��
	BlueTx();//�������Ϳ�ʼ
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

void ModeBlueImage(void)//��CCDͼ��
{
	basic_mode=4;
	Image_or_not=1;//��CCDͼ��ʼ������ͼ��flag,error,a_error,b_error,CurrentSteer
	Speed_or_not=57;//��������ֵ�����
	OLED_Init();//OLED��ʼ��
	BlueTx();//�������Ϳ�ʼ
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
void ModeOpenGo(void)//������
{
	basic_mode=2;
	targetspeed=230;
	SET_motor(100,100);
	OLED_Init();//OLED��ʼ��
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
	case 18://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 19://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 20://���٣�����
		straightspeed=230;transspeed=180;turnspeed=180;deadspeed=180;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 21://���٣�����
		straightspeed=230;transspeed=180;turnspeed=180;deadspeed=190;barspeed=180;
		//Speed_kc1=11000;sp_x1=0.0075;sp_x2=3.5;sp_x3=30;//error>0
		Speed_kc1=13000;sp_x1=0.005;sp_x2=2.5;sp_x3=30;//error<0
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 24://���٣�����
		straightspeed=200;transspeed=160;turnspeed=160;deadspeed=160;barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 25://���٣�����
		straightspeed=200;transspeed=165;turnspeed=165;deadspeed=165;barspeed=170;
		Speed_kc1=16000;sp_x1=0.005;sp_x2=2;sp_x3=30;
		Speed_kp_Left=8;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=8;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 26://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 27://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 28://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 29://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 30://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 31://���٣�����
		straightspeed=230;transspeed=190;turnspeed=190;deadspeed=190;barspeed=180;
		Speed_kc1=10000;sp_x1=0.006;sp_x2=2;sp_x3=30;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	}
}

