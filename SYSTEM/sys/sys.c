#include "sys.h" 


PID_Regulator_t Cir1_PID = Circle_Pid_Default;
PID_Regulator_t Cir2_PID = Circle_Pid_Default;

uint16_t Check_Val = 0;//��⸡��hal����⸡��λ��hal
u16 adc_after_val[3];

/*****��ʱ���жϣ�ÿ10ms�ж�һ�Σ�����pid�����pwm**/
/*void Control_Out(u8 arr, u8 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM2_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//����жϴ�����λ
	
	TIM_DeInit(TIM2);
	TIM2_TimeBaseStructure.TIM_Period            = arr;
	TIM2_TimeBaseStructure.TIM_Prescaler         = psc;
	TIM2_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM2_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, & TIM2_TimeBaseStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
}


void TIM2_IRQHandler(void)   //TIM2�ж�
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	 {
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
     }
	 
      ADC_Filter(adc_after_val);
	  Check_Val = adc_after_val[0];
	  Cir2_PID.fdb = adc_after_val[1];
	  Cir1_PID.fdb = adc_after_val[2];	
	 
	if(Check_Val < Th_Val)
	{
		//Cir1_PID.Calc;
		PID_Calc(&Cir1_PID);
		if(Cir1_PID.output < 0)       {Circle_Control(A, dir1, -Cir1_PID.output);}
		else if(Cir1_PID.output >= 0) {Circle_Control(A, dir2, Cir1_PID.output);}
		else                          {Circle_Control(A, 2, 0);}
		
		PID_Calc(&Cir2_PID);
		if(Cir2_PID.output < 0)       {Circle_Control(B, dir1, -Cir2_PID.output);}
		else if(Cir2_PID.output >= 0) {Circle_Control(B, dir2, Cir2_PID.output);} 
		else                          {Circle_Control(B, 2, 0);}
	}
	else 
	{
		Circle_Control(A, 2, 0);
		Circle_Control(B, 2, 0);
	}
}
*/





