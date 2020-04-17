
/**
  ******************************************************************************
  * @file		LCD1602X4bit.c
  * @author	Ho Sy Hoang
  * @date		17 April 2020
  ******************************************************************************
  */

#include "LCD1602x4bit.h"

static LCD_PIN_TypeDef RS,EN,D4,D5,D6,D7;


void setPin(LCD_PIN_TypeDef *Struct,uint8_t GPIO_Pin);
void pinConfig(void);
void lcd_write_4bit_data(uint8_t data4b);




//---------------------------------------------------------------------------------Configure Pin For LCD---------------------------------------------------------------------------------//

void pinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();	         // enable clock of GPIOA and GPIOB

	GPIO_InitStruct.Pin = RS.PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(RS.PORT, &GPIO_InitStruct);             // initialize RS PIN
	GPIO_InitStruct.Pin = EN.PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(EN.PORT, &GPIO_InitStruct);             // initialize EN PIN
	GPIO_InitStruct.Pin = D4.PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(D4.PORT, &GPIO_InitStruct);             // initialize D4 PIN
	GPIO_InitStruct.Pin = D5.PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(D5.PORT, &GPIO_InitStruct);             // initialize D5 PIN
	GPIO_InitStruct.Pin = D6.PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(D6.PORT, &GPIO_InitStruct);             // initialize D6 PIN
	GPIO_InitStruct.Pin = D7.PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(D7.PORT, &GPIO_InitStruct);             // initialize D7 PIN
}
void setPin(LCD_PIN_TypeDef *Struct,uint8_t GPIO_Pin)
{
	uint8_t HB=0,LB=0;
	uint16_t bit=1;
	HB=(GPIO_Pin&0xf0)>>4;
	LB=GPIO_Pin&0x0f;
	if(HB==1)            // Set port
	{
		Struct->PORT=GPIOA;
	} else if(HB==2){
		Struct->PORT=GPIOB;
	}
	bit<<=LB;
	Struct->PIN=bit;     // Set pin
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


void lcd_init (uint8_t RS_PIN,uint8_t EN_PIN,uint8_t D4_PIN,uint8_t D5_PIN,uint8_t D6_PIN,uint8_t D7_PIN)
{
	/*----------   Pin Configure    --------*/
	/*---------- RW connect to GND ---------*/
	setPin(&RS,RS_PIN);
	setPin(&EN,EN_PIN);
	setPin(&D4,D4_PIN);
	setPin(&D5,D5_PIN);
	setPin(&D6,D6_PIN);
	setPin(&D7,D7_PIN);
	pinConfig();
	/*--- initialize Delay_us Function ----*/
	Delay_Us_Init();
	/*-----------LCD Command Init----------*/
	lcd_send_cmd (0x33); /* set 4-bits interface */
	lcd_send_cmd (0x32);
	HAL_Delay(30);
	lcd_send_cmd (0x28); /* start to set LCD function */
	HAL_Delay(30);
	lcd_send_cmd (0x06); /* set entry mode */
	HAL_Delay(30);
	lcd_send_cmd (0x0c); /* set display to on */	
	HAL_Delay(30);
	lcd_send_cmd (0x02); /* move cursor to home and set data address to 0 */
	HAL_Delay(30);
	lcd_send_cmd (0x80);
	HAL_Delay(30);
	lcd_send_cmd (0x01); /* clear display */
	HAL_Delay(50);
}




void lcd_write_4bit_data(uint8_t data4b)
{
	D4.PORT->BRR=D4.PIN;
	D5.PORT->BRR=D5.PIN;
	D6.PORT->BRR=D6.PIN;
	D7.PORT->BRR=D7.PIN;
	if (data4b & 0x01) D4.PORT->BSRR = D4.PIN;    //D4 = LSB
	if (data4b & 0x02) D5.PORT->BSRR = D5.PIN;
	if (data4b & 0x04) D6.PORT->BSRR = D6.PIN;
	if (data4b & 0x08) D7.PORT->BSRR = D7.PIN;    //D7 = MSB
}

void lcd_send_data(uint8_t data)
{
	RS.PORT->BSRR=RS.PIN;               // rs=1
	EN.PORT->BSRR=EN.PIN;
	lcd_write_4bit_data(data>>4);     //send 4 high-bit
	Delay_us(100);
	EN.PORT->BRR=EN.PIN;
	EN.PORT->BSRR=EN.PIN;
	lcd_write_4bit_data(data&0x0f);     //send 4 high-bit
	Delay_us(100);
	EN.PORT->BRR=EN.PIN;
}

void lcd_send_cmd(uint8_t cmd)
{
	RS.PORT->BRR=RS.PIN;               // rs=0
	EN.PORT->BSRR=EN.PIN;
	lcd_write_4bit_data(cmd>>4);     //send 4 high-bit
	Delay_us(100);
	EN.PORT->BRR=EN.PIN;
	EN.PORT->BSRR=EN.PIN;
	lcd_write_4bit_data(cmd&0x0f);     //send 4 high-bit
	Delay_us(100);
	EN.PORT->BRR=EN.PIN;
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); //clear display
}

void lcd_goto_XY (int row, int col)
{
	uint8_t pos_Addr;
	if(row == 1)                       // First line = 1 , Second line = 2 
	{
		pos_Addr = 0x80 + row - 1 + col;
	}
	else if(row == 2)
	{
		pos_Addr = 0x80 | (0x40 + col);
	}
	lcd_send_cmd(pos_Addr);
}
void lcd_make_syms(char dat[])
{
	lcd_send_cmd(0x40);
char i;
	for(i=0;i<8;i++)
	{
	lcd_send_data(dat[i]);
	}
}
void lcd_display_number(unsigned char number)  // 0-99
{
	unsigned char c,dv;
	c = number/10;
	dv = number%10;
	lcd_send_data(c+48);
	lcd_send_data(dv+48);
}

/**********************************************************/

/**********************************************************/

/**********************************************************/
uint32_t Delay_Us_Init(void) {
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

     /* 3 NO OPERATION instructions */
     __ASM volatile ("NOP");
     __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
     if(DWT->CYCCNT)
     {
       return 0; /*clock cycle counter started*/
     }
     else
  {
    return 1; /*clock cycle counter not started*/
  }
}
/**********************************************************/
/**********************************************************/
