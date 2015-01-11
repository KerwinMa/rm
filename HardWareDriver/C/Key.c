
#include "config.h"             //�������е�����ͷ�ļ�
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
Tim.c file
��д�ߣ�С��  (Camel)
����E-mail��375836945@qq.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2014-01-28
���ܣ�
1.����IO�ڳ�ʼ��
2.�ⲿ�ְ����Ĺ����Ƕ��⿪�ŵģ���û�ж��� �书�ܣ�������ɷ��Ӱ�
3.ң���ϵ�Key1��ʱ���ã����µ����������Զ��壬��������Ϊƫ�����ڣ�һ�������ȵ�
------------------------------------
*/


extern int  Pitch_Offest;
extern int  Roll_Offest;

/********************************************
              Key��ʼ������
���ܣ�
1.����Key�ӿ�IO���뷽��

������
Key�ӿڣ�
Key1-->PB2
Key2-->PB1
Key3-->PB3
********************************************/
void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PB1,PB3,PA8) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);

	/* config the NVIC(Mode-->PB1) */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* config the NVIC(������-->PB3) */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* config the NVIC(������-->PA8) */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure Mode-->PB1 as output push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure ������-->PB3 as output push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure ������-->PA8 as output push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	//Mode-->PB1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;            //�趨�ⲿ�ж�1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //�趨�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�趨�½��ش���ģʽ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//������-->PB3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;            //�趨�ⲿ�ж�1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //�趨�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�趨�½��ش���ģʽ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//��-->PA8
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;              //�趨�ⲿ�ж�1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�趨�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�趨�½��ش���ģʽ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}



extern char Lockflag;

void EXTI1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
    
		ClibraFlag = FAIL;
		
		
		 #ifdef debugprint
	    	printf("keymodepress...\r\n");
		 #endif
		
		
   	EXTI_ClearITPendingBit(EXTI_Line1);     //����жϱ�־λ
	}
}

void EXTI3_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		
    Lockflag = !Lockflag;
		LedSet(led5,Lockflag);
		
				
		 #ifdef debugprint
	    	printf("keyaddepress...\r\n");
		 #endif
		
		
		
		EXTI_ClearITPendingBit(EXTI_Line3);     //����жϱ�־λ
		
	}
}


void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line8) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{	    

		
		
		
			
		 #ifdef debugprint
	    	printf("keysubepress...\r\n");
		 #endif
		
		
		
		EXTI_ClearITPendingBit(EXTI_Line8);     //����жϱ�־λ
	}
}




