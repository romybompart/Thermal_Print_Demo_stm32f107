#ifndef __HEADER_HARDWARE_H
#define __HEADER_HARDWARE_H
#include "stm32f10x_conf.h"
#include "fonts.h"

/** @addtogroup Header Pin Assignment and ports.
  * @{
  */
	
/** @brief SPI Pins pin configuration 
  *	@addtogroup Header Pin controls
  * @{
	*/
#define HEADER_SPI				SPI1
#define	HEADER_SPI_Cmd		RCC_APB2Periph_SPI1
#define HEADER_PIN_MOSI		GPIO_Pin_5
#define HEADER_PORT_MOSI	GPIOB
#define HEADER_PIN_MISO		GPIO_Pin_4
#define HEADER_PORT_MISO	GPIOB
#define HEADER_PIN_SCK		GPIO_Pin_3
#define HEADER_PORT_SCK		GPIOB
#define HEADER_SPI_PORT_CLK 	RCC_APB2Periph_GPIOB
/**
	* @}
  * @brief Control Pins pin configuration 
  *	@addtogroup Header Pin controls
  * @{
	*/
#define VH_PIN						GPIO_Pin_6
#define VH_PORT						GPIOC

#define LATCH_PIN					GPIO_Pin_8
#define LATCH_PORT        GPIOE
#define PEM_PIN						GPIO_Pin_9
#define PEM_PORT					GPIOE

#define STB1_PIN					GPIO_Pin_10
#define	STB1_PORT					GPIOE
#define STB2_PIN					GPIO_Pin_11
#define	STB2_PORT					GPIOE
#define STB3_PIN					GPIO_Pin_12
#define	STB3_PORT					GPIOE
#define STB4_PIN					GPIO_Pin_13
#define	STB4_PORT					GPIOE
#define STB5_PIN					GPIO_Pin_14
#define	STB5_PORT					GPIOE
#define STB6_PIN					GPIO_Pin_15
#define	STB6_PORT					GPIOE
#define HEADER_CRTL_PORT_CLK 	RCC_APB2Periph_GPIOE

#define PH11_PIN					GPIO_Pin_9  //IN1 pin at L293D
#define	PH11_PORT					GPIOB
#define PH12_PIN					GPIO_Pin_8  //IN3 pin at L293D
#define	PH12_PORT					GPIOD
#define PH21_PIN					GPIO_Pin_10 //IN2 pin at L293D
#define	PH21_PORT					GPIOD
#define PH22_PIN					GPIO_Pin_9  //IN4 pin at L293D
#define	PH22_PORT					GPIOD
#define HEADER_M1CRTL_PORT_CLK 	RCC_APB2Periph_GPIOB
#define HEADER_M2CRTL_PORT_CLK 	RCC_APB2Periph_GPIOD

#define ENABLE1_PIN				GPIO_Pin_7
#define ENABLE1_PORT			GPIOC
#define ENABLE2_PIN				GPIO_Pin_11
#define ENABLE2_PORT			GPIOD
#define HEADER_E1CRTL_PORT_CLK 	RCC_APB2Periph_GPIOC
#define HEADER_E2CRTL_PORT_CLK 	RCC_APB2Periph_GPIOD

	/**
		* @}
		*/
#define HEADER_PORT_GROUP_B	GPIOB
#define HEADER_PORT_GROUP_C	GPIOC
#define HEADER_PORT_GROUP_D GPIOD
#define HEADER_PORT_GROUP_E	GPIOE
/**
  * @}
  */
	
#define MOTOR_ENABLE1			GPIO_SetBits(ENABLE1_PORT,ENABLE1_PIN)
#define MOTOR_ENABLE2			GPIO_SetBits(ENABLE2_PORT,ENABLE2_PIN)
#define MOTOR_DISABLE1		GPIO_ResetBits(ENABLE1_PORT,ENABLE1_PIN)
#define MOTOR_DISABLE2		GPIO_ResetBits(ENABLE2_PORT,ENABLE2_PIN)
#define ENABLE_VH					GPIO_SetBits(VH_PORT,VH_PIN)
#define	DISABLE_VH				GPIO_ResetBits(VH_PORT,VH_PIN)
#define LATCH_ENABLE			GPIO_ResetBits(LATCH_PORT,LATCH_PIN)
#define LATCH_DISABLE			GPIO_SetBits(LATCH_PORT,LATCH_PIN)

#define ASK4PAPER					GPIO_ReadInputDataBit(PEM_PORT,PEM_PIN)

#define ERROR_FEED_PITCH	((uint8_t) 0x01)
#define	IS_PAPER					0x01
#define	NO_PAPER					0x00
#define FORWARD						0x00
#define BACKWARD					0x01

/**
	* @}
  * @brief Thermistor variables and definition 
  *	@addtogroup Thermistor
  * @{
	*/
#define THERMISTORPIN			GPIO_Pin_0
#define THERMISTORPORT		GPIOC
#define RCCTHERMISTORPORT	RCC_APB2Periph_GPIOC

#define BCoefficent				3950
#define RthNominal				30000
#define	TempNominal				25 
#define ADCResolution			4096
#define	SeriesResistor		10000

#define NumSamples				10

#define KELVIN						1
#define CELSIUS						0

/**
  * @}
  */


	

extern uint8_t Header_Init(void);
extern void Init_ADC(void);
extern void Init_PrinterSPI(void);
extern void PrintDots(uint16_t *Array, uint8_t max) ;
extern uint8_t feed_pitch(uint64_t lines, uint8_t forward_backward);
extern void ClearPrinterBuffer(void);
static void Motor_Stepper_Pos(uint8_t Position);
static void Delay_motor(__IO uint32_t nCount);
static void Printer_SendWorld(uint16_t World);

#endif

