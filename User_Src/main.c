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
main.c file
��д�ߣ�С��  (Camel)
����E-mail��375836945@qq.com
���뻷����MDK-Lite  Version: 4.23
����ʱ��: 2014-01-28
���ܣ�
------------------------------------
*/
#include "config.h"             //�������е�����ͷ�ļ�

// uint32_t startTime[5],execTime[5];
// uint32_t realExecPrd[5];

#define debugprint


uint32_t t0,t1;

extern char Lockflag;

int main(void)
{
	static char ledsta;
	SystemClock_HSI(9);           //ϵͳʱ�ӳ�ʼ����ʱ��Դ�ڲ�HSI
	cycleCounterInit();				    // Init cycle counter
	SysTick_Config(SystemCoreClock / 1000);	//SysTick����ϵͳtick��ʱ������ʼ�����жϣ�1ms
	UART1_init(SysClock,uart1baudSet); //����1��ʼ��
  NVIC_INIT();	                //�жϳ�ʼ��
  STMFLASH_Unlock();            //�ڲ�flash����
  LoadParamsFromEEPROM();				//����ϵͳ�������ñ�
  LedInit();	                  //IO��ʼ��
  Adc_Init();										//ҡ��AD��ʼ��
	KeyInit();										//������ʼ��
 	NRF24L01_INIT();              //NRF24L01��ʼ��
  SetTX_Mode();                 //������ģ��Ϊ����ģʽ
  controlClibra();							//ң��ҡ��У׼ 
	 
	TIM4_Init(SysClock,TIME4_Preiod);	  //��ʱ��4��ʼ������ʱʱ�䵥λ��(TIME4_Preiod)΢��
	crazeponyLocker = Lock;             //����
	
	LedSet(led2,1);
	LedSet(led3,1);
	LoadRCdata(America);               //ҡ�˸�ֵ
	UnlockCrazepony();
	crazeponyLocker = UnLock;         //�����ɻ�
	 
  LedSet(led2,0);
	LedSet(led3,0);
	 
  while (1)             
	{ 
		
if(flag10Hz == 1)  //10Hz ��Ƶ����
		{		
			flag10Hz = 0;
			ledsta = !ledsta;
			LedSet(signalLED,ledsta);				          //�ź�������ʾ
			
			if(Lockflag)crazeponyLocker = UnLock;     //ʵʱ������־
		  else crazeponyLocker = Lock;
			
			if(ClibraFlag == FAIL)controlClibra();	  //ң��ҡ��У׼
			
      #ifdef debugprint
				printf("thr -->%d\r\n",Throttle);
				printf("pitch -->%d\r\n",Pitch);
				printf("roll -->%d\r\n",Roll);
				printf("yaw -->%d\r\n",Yaw);
				printf("-------------\r\n");
			#endif
			
		}
		
		
if(flag50Hz == 1)//
		{		
			flag50Hz = 0;
			NRF24L01_TxPacket(TxBuf);//9ms
		}
		
if(flag80Hz)//ҡ��ADֵ�ɼ� 80Hz 12.5ms
		{
			flag80Hz = 0;
			
			t0 = micros();
			
			LoadRCdata(America);//ҡ�˸�ֵ
			
			TxBuf[0] = (Throttle)&0xff;
      TxBuf[1] = ((Throttle)>>8);           
			
      TxBuf[2] = (Pitch&0xff);             
			TxBuf[3] = ((Pitch)>>8);                  
			                                          
      TxBuf[4] = (Roll&0xff);                
			TxBuf[5] = ((Roll)>>8);
      
			TxBuf[6] = (Yaw&0xff);                 
			TxBuf[7] = (Yaw>>8); 
	
			t1 = micros() - t0;        //0.8ms
		}
		
		
	}
	
	
	
}

