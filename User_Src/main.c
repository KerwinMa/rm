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
编写者：小马  (Camel)
作者E-mail：375836945@qq.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2014-01-28
功能：
------------------------------------
*/
#include "config.h"             //包含所有的驱动头文件

// uint32_t startTime[5],execTime[5];
// uint32_t realExecPrd[5];

#define debugprint


uint32_t t0,t1;

extern char Lockflag;

int main(void)
{
	static char ledsta;
	SystemClock_HSI(9);           //系统时钟初始化，时钟源内部HSI
	cycleCounterInit();				    // Init cycle counter
	SysTick_Config(SystemCoreClock / 1000);	//SysTick开启系统tick定时器并初始化其中断，1ms
	UART1_init(SysClock,uart1baudSet); //串口1初始化
  NVIC_INIT();	                //中断初始化
  STMFLASH_Unlock();            //内部flash解锁
  LoadParamsFromEEPROM();				//加载系统参数配置表
  LedInit();	                  //IO初始化
  Adc_Init();										//摇杆AD初始化
	KeyInit();										//按键初始化
 	NRF24L01_INIT();              //NRF24L01初始化
  SetTX_Mode();                 //设无线模块为接收模式
  controlClibra();							//遥控摇杆校准 
	 
	TIM4_Init(SysClock,TIME4_Preiod);	  //定时器4初始化，定时时间单位：(TIME4_Preiod)微秒
	crazeponyLocker = Lock;             //上锁
	
	LedSet(led2,1);
	LedSet(led3,1);
	LoadRCdata(America);               //摇杆赋值
	UnlockCrazepony();
	crazeponyLocker = UnLock;         //解锁飞机
	 
  LedSet(led2,0);
	LedSet(led3,0);
	 
  while (1)             
	{ 
		
if(flag10Hz == 1)  //10Hz 低频处理
		{		
			flag10Hz = 0;
			ledsta = !ledsta;
			LedSet(signalLED,ledsta);				          //信号闪灯提示
			
			if(Lockflag)crazeponyLocker = UnLock;     //实时加锁标志
		  else crazeponyLocker = Lock;
			
			if(ClibraFlag == FAIL)controlClibra();	  //遥控摇杆校准
			
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
		
if(flag80Hz)//摇杆AD值采集 80Hz 12.5ms
		{
			flag80Hz = 0;
			
			t0 = micros();
			
			LoadRCdata(America);//摇杆赋值
			
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

