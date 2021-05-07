
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonStruct;
DAC_InitTypeDef DAC_InitStruct ;

uint16_t adc_value;
void GPIO_Config(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//DAC 1.Channelin c�k�� Ledini verdik

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void ADC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonStruct.ADC_Mode=ADC_Mode_Independent  ;
	ADC_CommonStruct.ADC_Prescaler= ADC_Prescaler_Div4;

	ADC_CommonInit(&ADC_CommonStruct);

	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;
	ADC_Init(ADC1,&ADC_InitStruct);

	ADC_Cmd(ADC1,ENABLE);

}
void DAC1_Config(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStruct.DAC_OutputBuffer=DAC_OutputBuffer_Enable ;
	DAC_InitStruct.DAC_WaveGeneration=DAC_WaveGeneration_None ;
	DAC_InitStruct.DAC_Trigger=DAC_Trigger_None ;

	DAC_Init (DAC_Channel_1,&DAC_InitStruct);

	DAC_Cmd(DAC_Channel_1,ENABLE);

}
void delay(uint32_t time)
{
	while(time--);
}
uint16_t Read_ADC ()
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_28Cycles);

	ADC_SoftwareStartConv (ADC1);
	while (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)== RESET);
	return  ADC_GetConversionValue(ADC1);

}
int main(void)
{
GPIO_Config();
ADC_Config();
DAC1_Config();

  while (1)
  {

	  if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0 ))
	  	{
	  		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
	  			{
	  			adc_value=Read_ADC ();
	  			DAC_SetChannel1Data(DAC_Align_12b_R ,adc_value);
	  			GPIO_SetBits(GPIOA,GPIO_Pin_4);
	  			}
	  	}

	  else
	  {

		  DAC_SetChannel1Data(DAC_Align_12b_R ,0);
	  }




  }
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
