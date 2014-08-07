/**
  * @file    header_driver.c
  * @author  Romy D. Bompart. B. from HARDWARE DEPARTMENT
  * @company The Factory HKA, CA.
  * @version V1.0.0
  * @date    19-December-2013
  * @brief   This file provides all the printer header software.
    ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
 */

#include "header_hardware.h"
#include <math.h>


uint8_t current_pos = 1; 
uint16_t STBx[] = {STB1_PIN,STB2_PIN,STB3_PIN,STB4_PIN,STB5_PIN,STB6_PIN};

__IO uint16_t ADCConvertedValue;

/** 
  * @brief  Header Init function, this function configure every port pin used 
  *  to drive the printer header. 	
  */
uint8_t Header_Init(void)
{
	GPIO_InitTypeDef GPIO_Struct;
	/**
	*		Control OUTPUT pins configuration Part1: Port E Group
	**/
	RCC_APB2PeriphClockCmd(HEADER_CRTL_PORT_CLK,ENABLE);
	GPIO_Struct.GPIO_Pin = LATCH_PIN | STB1_PIN | STB2_PIN | STB3_PIN | STB4_PIN | \
												 STB5_PIN  | STB6_PIN;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(HEADER_PORT_GROUP_E, &GPIO_Struct);
	//Setting the initial condition
	GPIO_SetBits(LATCH_PORT,LATCH_PIN);
	GPIO_ResetBits(HEADER_PORT_GROUP_E, STB1_PIN | STB2_PIN | STB3_PIN | STB4_PIN  | \
													STB5_PIN | STB6_PIN);
  /**
	*		Control OUTPUT pins configuration Part 2: Port D Group
	**/
	RCC_APB2PeriphClockCmd(HEADER_M2CRTL_PORT_CLK,ENABLE);
	GPIO_Struct.GPIO_Pin = PH12_PIN | PH21_PIN | PH22_PIN | ENABLE2_PIN;
	GPIO_Init(HEADER_PORT_GROUP_D, &GPIO_Struct);
	GPIO_ResetBits(HEADER_PORT_GROUP_D,PH12_PIN | PH21_PIN | PH22_PIN | ENABLE2_PIN);
	/**
	*		Control OUTPUT pins configuration Part 2: Port C Group
	**/
	RCC_APB2PeriphClockCmd(HEADER_E1CRTL_PORT_CLK,ENABLE);
	GPIO_Struct.GPIO_Pin = ENABLE1_PIN | VH_PIN;
// 	GPIO_Struct.GPIO_Pin = ENABLE1_PIN;
// 	GPIO_Init(HEADER_PORT_GROUP_C, &GPIO_Struct);
// 	GPIO_Struct.GPIO_Pin = VH_PIN;
 	GPIO_Init(HEADER_PORT_GROUP_C, &GPIO_Struct);
	GPIO_ResetBits(HEADER_PORT_GROUP_C,ENABLE1_PIN|VH_PIN);
	/**
	*		Control OUTPUT pins configuration Part 2: Port B Group
	**/
	RCC_APB2PeriphClockCmd(HEADER_M1CRTL_PORT_CLK,ENABLE);
	GPIO_Struct.GPIO_Pin = PH11_PIN;
	GPIO_Init(HEADER_PORT_GROUP_B, &GPIO_Struct);
	GPIO_ResetBits(HEADER_PORT_GROUP_B,PH11_PIN);
	/**
	*		Control OUTPUT pins configuration Part1: Port E Group
	**/
	GPIO_Struct.GPIO_Pin = PEM_PIN;
  GPIO_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HEADER_PORT_GROUP_E, &GPIO_Struct);
	/**
	*		Analogic pin configuration: PORTC
	**/
	RCC_APB2PeriphClockCmd(RCCTHERMISTORPORT,ENABLE);
  GPIO_Struct.GPIO_Pin = THERMISTORPIN;
  GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz ;
  GPIO_Init(THERMISTORPORT,&GPIO_Struct);
	Init_ADC();
	/**
	*		SPI pins configuration which belows to port B
	**/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	AFIO->MAPR |=0x02000000; // it is because the library has a bug
	GPIO_Struct.GPIO_Pin = HEADER_PIN_MOSI | HEADER_PIN_SCK;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(HEADER_PORT_GROUP_B, &GPIO_Struct);
	/*SPI initialization*/
	Init_PrinterSPI();
	ClearPrinterBuffer();
	
	
	return ASK4PAPER;
}


void Init_ADC()
{
	ADC_InitTypeDef ADC_InitStructure;
	/* Configuring the ADC Clock System ----------------------------------------*/
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	
	/* Enable peripheral clocks ------------------------------------------------*/

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel10 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

}

void Init_PrinterSPI()
{
	SPI_InitTypeDef SPI_Struct;
	
	RCC_APB2PeriphClockCmd(HEADER_SPI_Cmd,ENABLE);
	SPI_Struct.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_Struct.SPI_Mode = SPI_Mode_Master; 
	SPI_Struct.SPI_DataSize = SPI_DataSize_16b; 
	SPI_Struct.SPI_CPOL = SPI_CPOL_High; 
	SPI_Struct.SPI_CPHA = SPI_CPHA_1Edge; 
	SPI_Struct.SPI_NSS = SPI_NSS_Soft; 
	SPI_Struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
	SPI_Struct.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_Init(HEADER_SPI, &SPI_Struct); 
	SPI_Cmd(HEADER_SPI, ENABLE);           /* Enable the SPI  */  
}

void Motor_Stepper_Pos(uint8_t Position)
{
	  Delay_motor(6700);
		switch(Position){
			case 0:
				GPIO_ResetBits(PH11_PORT,PH11_PIN);
				GPIO_ResetBits(PH12_PORT,PH12_PIN);
				GPIO_ResetBits(PH21_PORT,PH21_PIN);			
				GPIO_ResetBits(PH22_PORT,PH22_PIN);
			break;
			case 1:
				GPIO_SetBits(PH11_PORT,PH11_PIN);
				GPIO_ResetBits(PH12_PORT,PH12_PIN);
				GPIO_ResetBits(PH21_PORT,PH21_PIN);			
				GPIO_SetBits(PH22_PORT,PH22_PIN);
 			break;
			case 2:
				GPIO_SetBits(PH11_PORT,PH11_PIN);
				GPIO_ResetBits(PH12_PORT,PH12_PIN);
				GPIO_SetBits(PH21_PORT,PH21_PIN);			
				GPIO_ResetBits(PH22_PORT,PH22_PIN);
			break;
			case 3:
				GPIO_ResetBits(PH11_PORT,PH11_PIN);
				GPIO_SetBits(PH12_PORT,PH12_PIN);
				GPIO_SetBits(PH21_PORT,PH21_PIN);			
				GPIO_ResetBits(PH22_PORT,PH22_PIN);
			break;
			case 4:
				GPIO_ResetBits(PH11_PORT,PH11_PIN);
				GPIO_SetBits(PH12_PORT,PH12_PIN);
				GPIO_ResetBits(PH21_PORT,PH21_PIN);			
				GPIO_SetBits(PH22_PORT,PH22_PIN);
			break;
		}
}

void Delay_motor(__IO uint32_t nCount) 
{
  while(nCount--) { }
}

uint8_t feed_pitch(uint64_t lines, uint8_t forward_backward)
{
	uint8_t pos = current_pos;
	uint8_t restor =  ~forward_backward;
	
	restor &= 0x01;
	
	if(lines>0)
	{
		MOTOR_ENABLE1;
		MOTOR_ENABLE2;
		while(lines>0)
		{

			Motor_Stepper_Pos(pos);     /* 0.0625mm */

			if(pos >= 1 && pos <= 4)
        pos = pos + (1 - 2*forward_backward); // adding or subtracting
			if(pos < 1 || pos > 4)
				pos = pos + (4 - 8*restor); // restoring pos
			
			
			lines--;
		}
	  MOTOR_DISABLE1;
		MOTOR_DISABLE2;
	}
	else
	{
		return ERROR_FEED_PITCH;
  }
	current_pos = pos;
	return 0;
}


void PrintDots(uint16_t *Array, uint8_t characters) 
{
	uint8_t i=0,y=0, MAX = Maxcharacterperline;
	    
		ENABLE_VH;
			while((i<characters) && (i<MAX))
			{
				Printer_SendWorld(Array[i]);
				i++;
				Delay_motor(10);
			}
			
			while( i < MAX)
			{
				Printer_SendWorld(0x0000);
				i++;
				Delay_motor(10);
			}
			
			LATCH_ENABLE;
			Delay_motor(30);
			LATCH_DISABLE;
			Delay_motor(50);
			
      while(y<6)
			{
					GPIO_SetBits(HEADER_PORT_GROUP_E,STBx[y]);
					Delay_motor(8000);
					GPIO_ResetBits(HEADER_PORT_GROUP_E,STBx[y]);
				  Delay_motor(80);
					y++;
			}
			feed_pitch(1,BACKWARD);
		DISABLE_VH;
}
	
void Printer_SendWorld(uint16_t World)
{
		SPI_I2S_SendData(HEADER_SPI,World);
}


void ClearPrinterBuffer()
{
	uint8_t i= 0;
	
	 for(i=0;i<24;i++)
	   Printer_SendWorld(0x0000);
	
	LATCH_ENABLE;
	Delay_motor(30);
  LATCH_DISABLE;
	Delay_motor(50);
}

float temperature(uint8_t Grade_K_C)
{  
	float Rth, Temperature;
	ADCConvertedValue=0;
	/* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
				
				/*Thermistor Resistance at x Kelvin*/
        ADCConvertedValue = ADC_GetConversionValue(ADC1);
				Rth = (ADCResolution/ADCConvertedValue) - 1;
				Rth =  SeriesResistor/Rth;
				/*Thermistor temperature in Kelvin*/
				Temperature =  Rth / RthNominal ;
				Temperature =  log(Temperature);
				Temperature =  (1/BCoefficent) * (Temperature);
				Temperature =  (1/(TempNominal + 273.15)) + Temperature;
				if (Grade_K_C == KELVIN)
					return Temperature;
				else
				/*Thermistor temperature in Celcius*/
					return	(Temperature - 273.15);
}


