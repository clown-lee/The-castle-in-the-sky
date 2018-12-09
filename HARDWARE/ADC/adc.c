#include "adc.h"

/**********************************************
DATA: 2018/6/6

 Coded by LZ
*************************************************************************/
#define ADC_SAMPLE_NUM	10

u16 adc_value[3*ADC_SAMPLE_NUM];//ADC�ɼ�ֵ��Ż�����



//��ʼ��ADC��ʹ��DMA����
//ͨ��PA1\PA2\PA3
void Adc_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//ʹADC1ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMAʱ��
	
	//PA1\2\3 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//DMA ����
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;	 //ADC1->DR��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc_value;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;    //���ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = 3*ADC_SAMPLE_NUM;  //DMAͨ����DMA�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //���ڴ浽�ڴ洫��ر�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	  //ɨ��ģʽ�����ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 3;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	ADC_DMACmd(ADC1, ENABLE);//ʹ��ADC1 DMA
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//��������ת��ͨ����55.5����������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);	//1��ͨ��ת��һ�κ�ʱ21us 4��ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);	//��������ADC_SAMPLE_NUM
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);	//�ܹ���ʱ3*21*ADC_SAMPLE_NUM��64��=
	
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
 
//ADC��ֵ�˲�
void ADC_Filter(u16* adc_val)
{
	u16 i=0;
	u32 sum[3]={0,0,0};
	
	for(;i<ADC_SAMPLE_NUM;i++)
	{
		sum[0]+=adc_value[3*i+0];
		sum[1]+=adc_value[3*i+1];
		sum[2]+=adc_value[3*i+2];
	}
	adc_val[0]=sum[0]/ADC_SAMPLE_NUM;
	adc_val[1]=sum[1]/ADC_SAMPLE_NUM;
	adc_val[2]=sum[2]/ADC_SAMPLE_NUM;
}

/*****��ʱ���жϣ�ÿ10ms�ж�һ�Σ�ADC����*/
/*void ADC_IT(u8 arr, u8 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//����жϴ�����λ
	
	TIM_DeInit(TIM3);
	TIM3_TimeBaseStructure.TIM_Period            = arr;
	TIM3_TimeBaseStructure.TIM_Prescaler         = psc;
	TIM3_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM3_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	 {
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
     }
	  ADC_Filter(adc_after_val);
	  Check_Val = adc_after_val[0];
	  Cir2_PID.fdb = adc_after_val[1];
	  Cir1_PID.fdb = adc_after_val[2];
}
*/


