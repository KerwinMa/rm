#ifndef _tim_H_
#define _tim_H_
#include "stm32f10x.h"



#define TIME4_Preiod  1000

void TIM4_Init(char clock,int Preiod);//���ڼ��ϵͳ
void TIM3_Init(char clock,int Preiod);//��ʱ��3�ĳ�ʼ��
void TimerNVIC_Configuration(void);//��ʱ���ж�����������

extern u16 flag10Hz,flag50Hz,flag80Hz,flag100Hz;


#endif

