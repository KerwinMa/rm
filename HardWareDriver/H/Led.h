#ifndef _Led_H_
#define _Led_H_

#include "stm32f10x.h"

#define signalLED 1
#define led2      2
#define led3      3
#define led4      4
#define led5      5

void LedInit(void);   //Led��ʼ�������ⲿ����
void LedSet(u8 ch,char sta);//LED�����ر�


#endif

