/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void ConfigureGpios()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); // zegar dla portu GPIO z którego wykorzystany zostanie pin jako wejœcie ADC (PA1)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // zegar dla

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure1);

	GPIO_InitTypeDef GPIO_InitStructure2;
		GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure2);

}
void ConfigureTimers()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 33999;
	TIM_TimeBaseStructure.TIM_Prescaler = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down ;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}
void ConfigureAdcs()
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);

	ADC_Cmd(ADC1, ENABLE);
}

int VoltageToHumidityConversion(int temp)
{
	double voltage=(double)temp/10000;
	const double maxHumidity = 1.48;
	const double minHumidity = 2.95;

	int humidity = 100*(1-(voltage-maxHumidity)/(minHumidity-maxHumidity));

	return humidity;
}

int MeasureHumidity()
{
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	double adcResult = ((double)ADC_GetConversionValue(ADC1)/(double)4095)*2.95;//some magic ints

	return VoltageToHumidityConversion((int)(10000*adcResult));
}

int isOutOfWater;
int a=0;
void PumpWater(int previousHumidity)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	TIM_Cmd(TIM4, ENABLE);
	if(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update))
	{
				if(a==1)
				{
					int currentHumidity=MeasureHumidity();
					if(currentHumidity==previousHumidity)
					{
						isOutOfWater=1;
						GPIO_SetBits(GPIOA,GPIO_Pin_3);
						GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
					}

				}
				a=1;
				TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	}
}

void PumpControl(int humidity)
{
	if(humidity<50)
		PumpWater(humidity);
	else
	{
		TIM_Cmd(TIM4, DISABLE);
		a=0;
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
	}
}

int main(void)
{
	ConfigureTimers();
	ConfigureGpios();
	ConfigureAdcs();
	isOutOfWater=0;
	for(;;)
	{
		int humidity = MeasureHumidity();
		if(isOutOfWater==0)
		PumpControl(humidity);
	}
}
