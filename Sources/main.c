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
	else if(mode>=8)   ModeBlueImage(); //��CCDͼ��
	else if(mode>=4)   ModeBlueSpeed(); //���ٶ�ͼ��
	else if(mode>=2)   ModeOpenGo();//������
}

void Direction_Control(void) //ת����ƺ���
{
	ImageCapture(A,B,C);  //��ͼ��A-60��B-30��C-��Զ
	PixelScan_B(); //ͼ����-B
	PixelScan_A();//ͼ����-A
	ErrorCalculate_A();//ƫ��ֵ����-A
	ErrorCalculate();//ƫ��ֵ����-B
	
	//pre_error=error;  //����errorֵ
	//TrendCalculate(); //ERROR���Ƽ���
	//EnterJudge();  //�����ж�
	
	Steer_PDSet();  //���PD����
	SteerControl(); //�������
}

void ModeGo(void)//����
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

void ModeBlueImage(void)//��CCDͼ��
{
	Image_or_not=1;//��CCDͼ��ʼ������ͼ��flag,error,a_error,b_error,CurrentSteer
	Speed_or_not=52;//��������ֵ�����
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
void ModeBlueSpeed(void)//���ٶ�ͼ��
{
	Image_or_not=30;//�Ӷ��ֵ��ʼ������CurrentSteer,csl,csr,tsl,tsr
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
void ModeOpenGo(void)//������
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

