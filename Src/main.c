#include <string.h>
#include <stdio.h>
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RxBufferSize   0xFF
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer;
uint8_t RxBuffer[RxBufferSize];
uint8_t RxCounter = 0;
uint8_t len , i;
char buffer[50];
RCC_ClocksTypeDef RCC_Clock;

char receive(void);
void SysInformation(void);
void Menu(void);


void Init_Usart()
{
	//USART Configuration
	/*
	BaudRate = 9600
	WordLeght = 8 bits
	Stop Bits = 1
  Parity = None
  Hardware Flow control = None	
	*/
	
	USART_InitTypeDef USART_Structure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	USART_Structure.USART_BaudRate = 9600;
	USART_Structure.USART_WordLength = USART_WordLength_8b;
  USART_Structure.USART_StopBits = USART_StopBits_1;
  USART_Structure.USART_Parity = USART_Parity_No;
	USART_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_Structure);
	USART_Cmd(UART4, ENABLE);
}



void Delay(__IO uint32_t nCount) 
{
  while(nCount--) { }
}


void putstring(char *string) 
{
	len = strlen(string);
	for(i=0;i<len;i++)
	{
		USART_SendData(UART4,string[i]);
		while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
	}
}

void SysInformation(void)
{
	 RCC_GetClocksFreq(&RCC_Clock);
	 putstring("\n\n\r ------------System Information----------------\n\r");
	 putstring("\n\n\r UART Configurated : YES\n\r");
	 putstring("\n\n\r STEPPER MOTOR PINs Configurated : YES\n\r");
	 sprintf(buffer,"\n\n\r SYCLK : %u\n\r", RCC_Clock.SYSCLK_Frequency);
	 putstring(buffer);
	 sprintf(buffer,"\n\n\r HCLK Clock : %u\n\r", RCC_Clock.HCLK_Frequency);
	 putstring(buffer);
	 sprintf(buffer,"\n\n\r Cortex Clock : %u\n\r", RCC_Clock.HCLK_Frequency/8);
	 putstring(buffer);
}

void Menu(void)
{
		 putstring("\n\n\r ---- Terminal Menu STM32F107 ---- \n\r");
		 putstring("\n\n\r 1.- Press numbers from 1 to 4 to steps motor\n\r");
	   putstring("\n\n\r 2.- Press S to display the clock system information\n\r");
		 putstring("\n\n\r 3.- Press M to display the Menu \n\n\r");
}


char receive(void) {
	while(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET)	{	}
  RxBuffer[0] = USART_ReceiveData(UART4);
  USART_SendData(UART4,RxBuffer[0]);
  return RxBuffer[0];
}



int main()
{
	int error;
	error = Header_Init();
 while(1)
	{
		if( IS_PAPER  == error ) 
		{
			ThermalPrint("        TICKET          \0");
			ThermalPrint("  THIS IS A TEST			 \0");
			feed_pitch(150,BACKWARD);

			error = ASK4PAPER;
		}
   }
}
