#include "sys.h"


int main()
 {  
	delay_init();
	Adc_Init();
	//ADC_IT((u8)1000, (u8)7199);  //ADC�жϲɼ�
	Motor_gpio_Init();
	TIM1_PWM_Init((u8)7199, 0);
	//Control_Out((u8)100, (u8)7199); //Ƶ��72Mhz,72M/7200=10K��1ms�ɼ�һ���ж�
	 
	//Cir1_PID = Circle_Pid_Default;
	//Cir2_PID = Circle_Pid_Default;
/********��ȦPID������ʼ����������Ҫ���˲���*********/

	Cir1_PID.ref  = 3080;   //��Ȧ��λֵ
	Cir1_PID.kp   = 10.0f; //10
	Cir1_PID.ki   = 0.0f;
	Cir1_PID.kd   = 0.0f;
	
	Cir2_PID.ref  = 3100;
	Cir2_PID.kp   = 10.0f;
	Cir2_PID.ki   = 0.0f;
	Cir2_PID.kd   = 0.0f;
/*************************************************/
	while(1)
	{  
	  Circle_Control(A, 0, 5000);
	  Circle_Control(B, 0, 5000);
	}
	
 }
