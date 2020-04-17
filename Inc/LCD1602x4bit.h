/**
  ******************************************************************************
  * @file		LCD1602X4bit.h
  * @author	Ho Sy Hoang
  * @date		17 April 2020
  ******************************************************************************
  */
	
	//my english is a bit bad, so please understand me =))
#ifndef __LCD1602x4bit_H
#define __LCD1602x4bit_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

typedef struct 
{
	uint16_t PIN;
	GPIO_TypeDef *PORT;
} LCD_PIN_TypeDef;


//GPIO PIN define
// if you want to use ports C or D just define them and change a bit in the setPin function in the LCD1602x4bit.b file
// 4 high-bit use to contain the port
// 4 low-bit use to contain the GPIO Pin
#define PA0 0x10
#define PA1 0x11
#define PA2 0x12
#define PA3 0x13
#define PA4 0x14
#define PA5 0x15
#define PA6 0x16
#define PA7 0x17
#define PA8 0x18
#define PA9 0x19
#define PA10 0x1A
#define PA11 0x1B
#define PA12 0x1C
#define PA13 0x1D
#define PA14 0x1E
#define PA15 0x1F
#define PB0 0x20
#define PB1 0x21
#define PB2 0x22
#define PB3 0x23
#define PB4 0x24
#define PB5 0x25
#define PB6 0x26
#define PB7 0x27
#define PB8 0x28
#define PB9 0x29
#define PB10 0x2A
#define PB11 0x2B
#define PB12 0x2C
#define PB13 0x2D
#define PB14 0x2E
#define PB15 0x2F






void lcd_send_data(uint8_t data);
void lcd_send_cmd(uint8_t cmd);

void lcd_display_number(unsigned char number);
void lcd_make_syms(char dat[]);
void lcd_goto_XY (int row, int col);
void lcd_clear_display (void);
void lcd_send_string (char *str);
void lcd_init (uint8_t RS_PIN,uint8_t EN_PIN,uint8_t D4_PIN,uint8_t D5_PIN,uint8_t D6_PIN,uint8_t D7_PIN);



/********************************************************************************************************************************/
uint32_t Delay_Us_Init(void);  

__STATIC_INLINE void Delay_us(volatile uint32_t microseconds)    // if you want to use this function in the main.c file  , you must call the Delay_Us_Init function first
{
  uint32_t clk_cycle_start = DWT->CYCCNT;

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}
/********************************************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/



