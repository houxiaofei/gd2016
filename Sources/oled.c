/*
 * oled.c
 *
 *  Created on: Mar 3, 2016
 *      Author: zhongwei
 */
/*
 * debug_oled0.c
 *
 *  Created on: Feb 28, 2016
 *      Author: Pascal
 */

#include "oled.h"


/**
 * 128X64I液晶底层驱动[6X8]字体库
 * [6X8]西文字符的字模数据 (纵向取模,字节倒序)
 * !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
 */
const unsigned char F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp 0
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *  10
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4  20
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >  30
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @  
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A  33
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G   
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H  40
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q   
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R  50
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [   
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55 60
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a  65
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f  70
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p  80
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z  90
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

/**
 * OLED0显示屏测试
 */
void OLED_Test(void)
{
    OLED_Fill(0x00);
	
//	int page;
//	for(page=0;page<8;page++){
//		OLED_SetPointer(page,0);
//		OLED_Str("    Hello!");
//		OLED_Num(70-page);
//		OLED_Char('A');
//		OLED_Num(88);
//	}
	OLED_SetPointer(0,5);
	OLED_Str("ts: ");
	OLED_Num(targetspeed);
	OLED_Str("  ");
	OLED_Num(tsl);
	OLED_Str("  ");
	OLED_Num(tsr);

	OLED_SetPointer(1,5);
	OLED_Str("key: ");
	OLED_Num(keymode);
	//	OLED_Str("  ");
	//	OLED_Str("RPID: ");
	//	OLED_Num(RPID);
//	OLED_Str("x1: ");
//	OLED_Num(sp_x1*10000);
//	OLED_Str("  ");
//	OLED_Str("kc1: ");
//	OLED_Num(Speed_kc1/100);
//	OLED_Str("ba: ");
//    OLED_Num(bar_edge[0]);
//    OLED_Str("  ");
//    OLED_Num(bar_edge[1]);
//    OLED_Str("  ");
//    OLED_Num(bar_edge[2]);
//    OLED_Str("  ");
//    OLED_Num(bar_edge[3]);
	
	OLED_SetPointer(2,5);
	OLED_Str("flag: ");
    OLED_Num(al_flag);
    OLED_Str("  ");
    OLED_Num(ar_flag);
    OLED_Str("  ");
    OLED_Num(bl_flag);
    OLED_Str("  ");
    OLED_Num(br_flag);
    
	OLED_SetPointer(3,5);
	OLED_Str("alb: ");
	OLED_Num(al_bar_flag);
	OLED_Str(" ");
	OLED_Str("arb: ");
	OLED_Num(ar_bar_flag);
	OLED_Str(" ");
	OLED_Str("ab: ");
	OLED_Num(a_bar_flag);
//	OLED_Str("aedg: ");
//    OLED_Num(al_edge_left);
//    OLED_Str("  ");
//    OLED_Num(al_edge_right);
//    OLED_Str("  ");
//    OLED_Num(ar_edge_left);
//    OLED_Str("  ");
//    OLED_Num(ar_edge_right);

    
	OLED_SetPointer(4,5);
	OLED_Str("edge: ");
    OLED_Num(al_edge);
    OLED_Str("  ");
    OLED_Num(ar_edge);
    OLED_Str("  ");
    OLED_Num(bl_edge);
    OLED_Str("  ");
    OLED_Num(br_edge);
    
	OLED_SetPointer(5,5);
	OLED_Str("cs: ");
	OLED_Num(csl);
	OLED_Str("  ");
	OLED_Num(csr);
//	OLED_Str("  ");
//	OLED_Str("rx ");
//	OLED_Num(RX_data);
//	OLED_Str("bedg: ");
//    OLED_Num(bl_edge_left);
//    OLED_Str("  ");
//    OLED_Num(bl_edge_right);
//    OLED_Str("  ");
//    OLED_Num(br_edge_left);
//    OLED_Str("  ");
//    OLED_Num(br_edge_right);
    
    OLED_SetPointer(6,5);
//        OLED_Str(" ");
//        OLED_Num(a_edg_err);
    OLED_Str("er: ");
    OLED_Num(a_error);
    OLED_Str(" ");
    OLED_Num(b_error);
    OLED_Str(" ");
    OLED_Num(error);
//      OLED_Str("s_flag: ");
//      OLED_Num(straight_flag);
//      OLED_Str(" ");
//      OLED_Num(trans_enter_flag);
//      OLED_Str(" ");
//      OLED_Num(trans_out_flag);
//      OLED_Str(" ");
//      OLED_Num(turn_flag);

    
    
	OLED_SetPointer(7,5);
//    OLED_Str("T: ")
//    OLED_Num(a_T);
//    OLED_Str("  ");
//    OLED_Num(b_T);
//    OLED_Str("  ");
//    OLED_Num(c_T);
	OLED_Str("csteer: ");
	CurrentSteer=TargetSteer;
	OLED_Num(CurrentSteer);
	OLED_Str("  ");
//	OLED_Str("bar: ");
//	OLED_Num(bar_flag);
//	OLED_Str("  ");
//	OLED_Num(ab_difference);
//	OLED_Str("bw: ");
//	OLED_Num(forwardleft);
//	OLED_Str("  ");
//	OLED_Num(backwardright);
//	OLED_Str("c: ");
//    OLED_Num(cl_flag);
//    OLED_Str("  ");
//    OLED_Num(cr_flag);
//    OLED_Str("  ");
//    OLED_Num(cl_edge);
//    OLED_Num(cr_edge);
}

/**
 * 在指定位置写入一组标准ASCII字符串
 * @param s 待写入的字符串
 */
void OLED_Str(char* s)
{
	while (*s) 
	{
		OLED_Char(*s);
		s++;
	}
}

/**
 * 在当前位置写入1个字符
 * @param c 要写入的数据
 */
void OLED_Char(unsigned char c)
{
    unsigned char line;
    c -= 32;
    for (line=0; line<6; line++)
    	OLED_Page(F6x8[c][line]);//从ACSII码表中读取字节，然后写入液晶
}

/********************************************************************//**
 * 显示整型变量
 * @param X 行（页）
 * @param Y 列
 * @param num 待显示的整数
 * @param N 待显示的整数位数
 ***********************************************************************/
void OLED_Num(int num)
{
  int i=0;
  const int N = 8;
  unsigned char n[N]={0};
  
  if(num<0) ///<处理负数
  {
	  OLED_Char('-');
	  num = -num;
  }
  
  for(i=0;i<N;i++) ///< 分割数字
  {
	  n[i] = num % 10;
	  num = num / 10;

  }
  
  for(i=N-1; n[i]==0 && i>0;i--); ///< 去除头零
  
  for(;i>=0;i--) ///< 打印出数字
  {
	  OLED_Char('0'+n[i]);
  }
}

/**
 * 初始化OLED
 * @param _img_row,_img_col 图像大小
 * @param _binMode 二值化模式
 * 		@arg FIXED_HALF 采用固定的中间阈值0x80
 * 		@arg HISTOGRAM 采用直方图自适应二值化
 */
void OLED_Init(void)        
{
	OLED_SCL=1;
	OLED_CS=1;	//预制SLK和SS为高电平   	
	
	OLED_RST=0;
	OLED_Delay(50);
	OLED_RST=1;
	
	OLED_Cmd(0xae);//--turn off oled panel
	OLED_Cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Cmd(0x81);//--set contrast control register
	OLED_Cmd(0xcf); // Set SEG Output Current Brightness
	OLED_Cmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_Cmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_Cmd(0xa6);//--set normal display
	OLED_Cmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_Cmd(0x3f);//--1/64 duty
	OLED_Cmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Cmd(0x00);//-not offset
	OLED_Cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_Cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Cmd(0xd9);//--set pre-charge period
	OLED_Cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Cmd(0xda);//--set com pins hardware configuration
	OLED_Cmd(0x12);
	OLED_Cmd(0xdb);//--set vcomh
	OLED_Cmd(0x40);//Set VCOM Deselect Level
	OLED_Cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Cmd(0x02);//
	OLED_Cmd(0x8d);//--set Charge Pump enable/disable
	OLED_Cmd(0x14);//--set(0x10) disable
	OLED_Cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_Cmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_Cmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);  //初始清屏
	OLED_SetPointer(0,0);  
}

/**
 * 向OLED发送命令, 命令参考SSD1306手册
 * @param cmd 要改送的命令
 */
void OLED_Cmd(BYTE cmd)
{
	BYTE i=8;
	OLED_CS=0;
	OLED_DC=0;
	OLED_SCL=0;
	//asm("nop");
	while(i--)
	{
		if(cmd&0x80)
		{
			OLED_SDA=1;
		}
		else
		{
			OLED_SDA=0;
		}
		OLED_SCL=1;
		asm("nop");
		//asm("nop");             
		OLED_SCL=0;
		cmd<<=1;
	}
	OLED_CS=1;
}

/**
 * 设置内容开始位置
 * @param col 显示屏横向分为128列,标记为0~127
 * @param page 显示屏纵向分为8个页面,标记为0~7
 */
void OLED_SetPointer(BYTE page,BYTE col)
{ 
	OLED_Cmd(0xb0+page); ///< Set Page Start Address for Page Addressing Mode (B0h~B7h)
	OLED_Cmd(col & 0x0F); ///< Set Column Start Address lower nibble
	OLED_Cmd(((col & 0xF0)>>4)|0x10); ///< Set Column Start Address upper nibble
}

/**
 * 用OLED_Page函数的方式填充屏幕
 * @param data 8位数据用于填充一页中的一列
 */
void OLED_Fill(BYTE data)
{
	BYTE y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_Cmd(0xb0+y);
		OLED_Cmd(0x01);
		OLED_Cmd(0x10);
		for(x=0;x<OLED_COL;x++)
			OLED_Page(data);
	}
}

/**
 * 绘制一页中的一列数据
 * @param data 8bit数据由高位向低位显示在一页中的一列
 */
void OLED_Page(BYTE data)
{
	BYTE i=8;
	OLED_CS=0;
	OLED_DC=1;
	OLED_SCL=0;
	//asm("nop");   
	while(i--)
	{
		if(data&0x80){OLED_SDA=1;}
		else{OLED_SDA=0;}
		OLED_SCL=1; 
		asm("nop");;
		//asm("nop");            
		OLED_SCL=0;;    
		data<<=1;    
	}
	OLED_CS=1;
}

/**
 * 延时
 * @param ms 延时时间,单位毫秒
 */
void OLED_Delay(WORD ms)
{                         
  WORD a;
  while(ms)
  {
    a=OLED_CLK; // 1ms内的时钟周期数
    while(a--)
    { }
    ms--;
  }
  return;
}



