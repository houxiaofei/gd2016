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
		if(mode>=16) ModeGo();	//�ܳ�
	}
	else
	{
		if(mode>=8)   ModeGoDebug(); //�ܳ�
		else if(mode==5)   ModeBlueImage(); //��CCDͼ��
		else if(mode==3)   ModeOpenGo();//������
		else if(mode==1) ModeSteerModify();//�������ֵ
	}
}

void Direction_Control(void) //ת����ƺ���
{
	ImageCapture(A,B,C);  //��ͼ��A-60��B-30��C-��Զ
	PixelScan_A();//ͼ����-A
	PixelScan_B(); //ͼ����-B
	PixelScan_C(); //ͼ����-C
	ErrorCalculate_A();//ƫ��ֵ����-A
	ErrorCalculate();//ƫ��ֵ����-B
	Steer_PDSet();  //���PD����
	SteerControl(); //�������
}

void ModeGoDebug(void)//����ģʽ��
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


void ModeGo(void)//����
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
	targetspeed=190;
	straightspeed=190;
	turnspeed=150;
	SET_motor(targetspeed,targetspeed);
	OLED_Init();//OLED��ʼ��
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


void ModeGoJudgeDebug(void)
{
	switch(mode){
	case 25://��һȦ
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 21://���⣬�ڶ�Ȧ
		straightspeed=230;turnspeed=170;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 23://�ϰ��ﲻ�����ڶ�Ȧ
		straightspeed=230;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 19://����Ȧ
		straightspeed=210;turnspeed=160;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 27://��һȦ,50
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		steer_amb_value=50;
		break;
	case 29://�ڶ�Ȧ��200��170
		straightspeed=200;turnspeed=170;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;sp_x1_right=0.025;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 31://��һȦ,200
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
	case 8://��һȦ
		straightspeed=250;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.02;sp_x2=4;sp_x3=35;
		Speed_kp_Left=15;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=15;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 4://���⣬�ڶ�Ȧ
		straightspeed=230;turnspeed=170;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 6://�ϰ��ﲻ�����ڶ�Ȧ
		straightspeed=230;turnspeed=190;
		Speed_kc1=13000;sp_x1=0.018;sp_x2=4;sp_x3=35;
		Speed_kp_Left=12;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=12;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	case 2://����Ȧ
		straightspeed=210;turnspeed=160;
		Speed_kc1=13000;sp_x1=0.015;sp_x2=4;sp_x3=35;
		Speed_kp_Left=10;Speed_ki_Left=0.2;Speed_kd_Left=1;
		Speed_kp_Right=10;Speed_ki_Right=0.2;Speed_kd_Right=1;
		break;
	}
}

