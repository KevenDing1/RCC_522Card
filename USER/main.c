#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ds18b20.h" 
#include "rc522.h"
//////////////////////////////////////////////////////////
//M1���֞�16���ȅ^��ÿ���ȅ^��4�K���K0���K1���K2���K3���M��
//�҂�Ҳ��16���ȅ^��64���K���^����ַ��̖0~63
//��0�ȅ^�ĉK0�����^����ַ0�K��������춴�ŏS�̴��a���ѽ��̻������ɸ���
//ÿ���ȅ^�ĉK0���K1���K2�锵���K������춴�Ŕ���
//ÿ���ȅ^�ĉK3����ƉK���^����ַ�K3��7��11....�����������ܴaA����ȡ���ơ��ܴaB��

/*******************************
*����˵����
*1--SS  <----->PF0
*2--SCK <----->PB13
*3--MOSI<----->PB15
*4--MISO<----->PB14
*5--����
*6--GND <----->GND
*7--RST <----->PF1
*8--VCC <----->VCC
************************************/
/*ȫ�ֱ���*/
unsigned char CT[2];				//������
unsigned char SN[4];        //����
unsigned char RFID[16];			//���RFID 

void DEV_Init(void);
void ShowID(u16 x,u16 y,u8 *p��u16 charColor��u16 bkColor)��
 int main(void)
 {	
  unsigned char status; 
	DEV_Init();
 	//POINT_COLOR=RED;										//��������Ϊ��ɫ 
	status=PcdRequest(PICC_REQALL,CT);
	if(status==MI_OK)                   	//�����ɹ�
			{
			 LCD_ShowString(0,30,200,16,16,"PcdRequest_MI_OK");
			 status=MI_ERR;
			 status = PcdAnticoll(SN);      	/*����ײ*/
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ 9600
	delay_init();	    	 //��ʱ������ʼ��	  
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD  
	InitRc522();			//��ʼ����Ƶ��ģ��
}
/*************************************
*�������ܣ���ʾ���ţ���ʮ��������ʾ
*������x,y����
*		p ���ŵĵ�ַ
*		charcolor �ַ���ɫ
*		bkcolor   ������ɫ
***************************************/
void ShowID(u16 x,u16 y,u8 *p��u16 charColor��u16 bkColor)
{
	u8 num[9];
	u8 i;
	/*************************************
	*�㷨������
	*p[i]Ϊu8��Ϊ8λ��0-255��
	*������16λ��ʾ������Ҫ��λ16��������ʾ
	*һ����num[0]��num[1]��ʾp[0];num[2]��num[3]��ʾp[1];
	*����num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');�ǽ�����9��ֵת���ɴ�д��ĸA-F
	*'7'��7��ascll��Ϊ55��A��ascll��Ϊ65
	**************************************/
	for(i=0;i<4;i++)
	{
		num[i*2]=p[i]/16;
		num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
		num[i*2+1]=p[i]%16;
		num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
	}
}