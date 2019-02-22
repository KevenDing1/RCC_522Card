#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ds18b20.h" 
#include "rc522.h"
//////////////////////////////////////////////////////////
//M1卡分為16個扇區，每個扇區由4塊（塊0、塊1、塊2、塊3）組成
//我們也將16個扇區的64個塊按絕對地址編號0~63
//第0扇區的塊0（即絕對地址0塊），他用於存放廠商代碼，已經固化，不可更改
//每個扇區的塊0、塊1、塊2為數據塊，可用於存放數據
//每個扇區的塊3為控制塊（絕對地址塊3、7、11....），包括了密碼A，存取控制、密碼B。

/*******************************
*连线说明：
*1--SS  <----->PF0
*2--SCK <----->PB13
*3--MOSI<----->PB15
*4--MISO<----->PB14
*5--悬空
*6--GND <----->GND
*7--RST <----->PF1
*8--VCC <----->VCC
************************************/
/*全局变量*/
unsigned char CT[2];				//卡类型
unsigned char SN[4];        //卡号
unsigned char RFID[16];			//存放RFID 

void DEV_Init(void);
void ShowID(u16 x,u16 y,u8 *p，u16 charColor，u16 bkColor)；
 int main(void)
 {	
  unsigned char status; 
	DEV_Init();
 	//POINT_COLOR=RED;										//设置字体为红色 
	status=PcdRequest(PICC_REQALL,CT);
	if(status==MI_OK)                   	//尋卡成功
			{
			 LCD_ShowString(0,30,200,16,16,"PcdRequest_MI_OK");
			 status=MI_ERR;
			 status = PcdAnticoll(SN);      	/*防冲撞*/
			}
	else
		   LCD_ShowString(0,30,200,16,16,"PcdRequest_MI_ERR");
	
	if(status==MI_OK)
			{
				LCD_ShowString(0,30,200,16,16,"PcdAnticoll_MI_ERR");
				status==MI_ERR;
				LED1=1;
				LED0=1;
				
			}
}

 void DEV_Init(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为 9600
	delay_init();	    	 //延时函数初始化	  
    LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD  
	InitRc522();			//初始化射频卡模块
}
/*************************************
*函数功能：显示卡号，以十六进制显示
*参数：x,y坐标
*		p 卡号的地址
*		charcolor 字符颜色
*		bkcolor   背景颜色
***************************************/
void ShowID(u16 x,u16 y,u8 *p，u16 charColor，u16 bkColor)
{
	u8 num[9];
	u8 i;
	/*************************************
	*算法分析：
	*p[i]为u8型为8位（0-255）
	*故若以16位表示，则需要两位16进制数表示
	*一下用num[0]和num[1]表示p[0];num[2]和num[3]表示p[1];
	*其中num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');是将大于9的值转换成大写字母A-F
	*'7'即7的ascll码为55，A的ascll码为65
	**************************************/
	for(i=0;i<4;i++)
	{
		num[i*2]=p[i]/16;
		num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
		num[i*2+1]=p[i]%16;
		num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
	}
}