/*    
      ____                      _____                  +---+
     / ___\                     / __ \                 | R |
    / /                        / /_/ /                 +---+
   / /   ________  ____  ___  / ____/___  ____  __   __
  / /  / ___/ __ `/_  / / _ \/ /   / __ \/ _  \/ /  / /
 / /__/ /  / /_/ / / /_/  __/ /   / /_/ / / / / /__/ /
 \___/_/   \__,_/ /___/\___/_/    \___ /_/ /_/____  /
                                                 / /
                                            ____/ /
                                           /_____/
led.c file
��д�ߣ�С��  (Camel)
����E-mail��375836945@qq.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2014-01-28
------------------------------------
*/

#include "Led.h"
#include "UART1.h"
#include "config.h"

/********************************************
              Led��ʼ������
���ܣ�
1.����Led�ӿ�IO�������
2.�ر�����Led(����Ĭ�Ϸ�ʽ)
������
��ӦIO=1������
********************************************/
void LedInit(void)
{
	  static u8 i;
    RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	
    RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��
  	RCC->APB2ENR|=1<<0;    //ʹ�ܸ���ʱ��	   
	
	  GPIOA->CRH&=0XFFF0FFFF;  //PA12�������
    GPIOA->CRH|=0X00030000;
    GPIOA->ODR|=1<<12;        //PA12����
  
    GPIOB->CRL&=0X0000FFFF;  //PB4,5,6,7�������
    GPIOB->CRL|=0X33330000;
    GPIOB->ODR|=15<<4;        //PB4,5,6,7����
    AFIO->MAPR|=2<<24;      //�ر�JATG,ǧ���ܽ�SWDҲ�رգ�����оƬ���ϣ��ײ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //PB4 Ĭ����JNTRST�ӿ�,���ڵ��Խӿڣ����ù���ΪPB4 IO��
	
	  for(i=1;i<=5;i++)LedSet(i,0);//��ʼ���ر�����LED
	
	  
}

void LedSet(u8 ch,char sta)
{
	switch(ch)
	{
		case signalLED:
			if(sta == 1) GPIO_SetBits(GPIOA, GPIO_Pin_12);
				else  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			break;
				
		case led2:
			if(sta == 1) GPIO_SetBits(GPIOB, GPIO_Pin_4);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_4);
			break;
		
		case led3:
			if(sta == 1) GPIO_SetBits(GPIOB, GPIO_Pin_5);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			break;
		
		case led4:
			if(sta == 1) GPIO_SetBits(GPIOB, GPIO_Pin_6);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_6);
			break;
		
		case led5:
			if(sta) GPIO_SetBits(GPIOB, GPIO_Pin_7);
				else  GPIO_ResetBits(GPIOB, GPIO_Pin_7);
			break;
	}
		
}

