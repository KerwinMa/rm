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
*/
/* Control.c file
��д�ߣ�С��  (Camel)
����E-mail��375836945@qq.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2014-01-28
���ܣ�
------------------------------------
*/

#include "adc.h"
#include "control.h"
#include "ConfigTable.h"
#include "Led.h"
#include "NRF24L01.h"
#include "delay.h"

int Throttle;
int Roll;
int Pitch;
int Yaw;

uint16_t Throttle_Calibra=0;
uint16_t Pitch_Calibra   =0;
uint16_t Roll_Calibra    =0;
uint16_t Yaw_Calibra     =0;//ҡ��У׼ֵ

char Lockflag = 0;


/*
ҡ�����ݲɼ�
�������Ϊ�����ֺ��ձ���
*/
void LoadRCdata(unsigned char RomoteMode)
{
  switch(RomoteMode)
  {
    /*����Ϊ�����ֵĶ�Ӧ��ϵ*/
 case America: 
      Throttle=1500 - (Throttle_Calibra-(1000 + (1000 - (1000*Get_Adc_Average(3,15))/4096)));//�ɼ�����ҡ�˵�λ�ã�����Ӳ��ԭ����Ҫ��100-�ɼ�ֵ
      Throttle=(Throttle<=1000)?1000:Throttle;               //Խ���ж�
      Throttle=(Throttle>=2000)?2000:Throttle;               //Խ���ж�

      Pitch= 1500 - (Pitch_Calibra - (1000 + (1000*Get_Adc_Average(1,15))/4096));//�ɼ�����ҡ�˵�λ�ã���ֵ����Ӧ�ĸ�������
      Pitch=(Pitch<=1000)?1000:Pitch;                 //Խ���ж�
      Pitch=(Pitch>=2000)?2000:Pitch;               //Խ���ж�

      Roll= 1500 - (Roll_Calibra - (1000 + (1000*Get_Adc_Average(0,15))/4096));//�ɼ����ҡ��λ�ã���ֵ����Ӧ�ĺ������
      Roll=(Roll<=1000)?1000:Roll;                //Խ���ж�
      Roll=(Roll>=2000)?2000:Roll;              //Խ���ж�

      Yaw= 1500 - (Yaw_Calibra - (1000 + (1000*Get_Adc_Average(2,15))/4096));//�ɼ����ҡ��λ�ã���ֵ����Ӧ��ƫ����
      Yaw=(Yaw<=1000)?1000:Yaw;                //Խ���ж�
      Yaw=(Yaw>=2000)?2000:Yaw;              //Խ���ж�
          break;
    /*����Ϊ�ձ��ֵĶ�Ӧ��ϵ*/
 case Japan:
	 
      Throttle=1500 - (Throttle_Calibra-(1000 + (1000 - (1000*Get_Adc_Average(1,15))/4096)));//�ɼ�����ҡ�˵�λ�ã�����Ӳ��ԭ����Ҫ��100-�ɼ�ֵ
      Throttle=(Throttle<=1000)?1000:Throttle;               //Խ���ж�
      Throttle=(Throttle>=2000)?2000:Throttle;               //Խ���ж�

      Pitch= 1500 - (Pitch_Calibra - (1000 + (1000*Get_Adc_Average(3,15))/4096));//�ɼ�����ҡ�˵�λ�ã���ֵ����Ӧ�ĸ�������
      Pitch=(Pitch<=1000)?1000:Pitch;                 //Խ���ж�
      Pitch=(Pitch>=2000)?2000:Pitch;               //Խ���ж�

      Roll= 1500 - (Roll_Calibra - (1000 + (1000*Get_Adc_Average(0,15))/4096));//�ɼ����ҡ��λ�ã���ֵ����Ӧ�ĺ������
      Roll=(Roll<=1000)?1000:Roll;                //Խ���ж�
      Roll=(Roll>=2000)?2000:Roll;              //Խ���ж�

      Yaw= 1500 - (Yaw_Calibra - (1000 + (1000*Get_Adc_Average(2,15))/4096));//�ɼ����ҡ��λ�ã���ֵ����Ӧ��ƫ����
      Yaw=(Yaw<=1000)?1000:Yaw;                //Խ���ж�
      Yaw=(Yaw>=2000)?2000:Yaw;              //Խ���ж�
      break;
  }
}


int8_t ClibraFlag;


void controlClibra(void)
{
	static u8 i;
	uint16_t sum[4]={0,0,0,0};
	static int8_t lednum=1;
  static int8_t clibrasumNum = 20;

	if((ClibraFlag == FAIL))//У׼ʧ��
	{

		for(i=0;i<clibrasumNum;i++)
		{
			
			sum[0] += 1000 + (1000 - (1000*Get_Adc_Average(3,15))/4096);
			sum[1] += 1000 + (1000*Get_Adc_Average(1,15))/4096;
			sum[2] += 1000 + (1000*Get_Adc_Average(0,15))/4096;
			sum[3] += 1000 + (1000*Get_Adc_Average(2,15))/4096;
			delay_ms(100);
			
			if(++lednum == led5 + 2)lednum = 2;
			LedSet(lednum - 1,0);
			LedSet(lednum ,1);
		}
		
		Throttle_Calibra = sum[0]/i;
		Pitch_Calibra    = sum[1]/i;
		Roll_Calibra     = sum[2]/i;
		Yaw_Calibra      = sum[3]/i;

		LoadRCdata(America);               //ҡ�˸�ֵ
		if((Throttle>=1520)||(Throttle<1480)||(Pitch>=1520)||(Pitch<=1480)||(Roll>=1520)||(Roll<=1480)||(Yaw>=1520)||(Yaw<=1480))
						ClibraFlag       = FAIL;//У׼ʧ��
		else 		ClibraFlag       = OK;//У׼�ɹ���־
		
					
		
		SaveParamsToEEPROM();
		LedSet(led2,0);LedSet(led3,0);LedSet(led4,0);LedSet(led5,0);
	 }	
	
}




void UnlockCrazepony(void)
{
		while((Throttle>1050)||(Roll>1050))//��������������������ͣ�������������
	 {
		 LoadRCdata(America);               //ҡ�˸�ֵ
		 NRF24L01_TxPacket(TxBuf);//9ms
	 }
}
