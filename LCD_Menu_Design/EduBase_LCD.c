/**
 * @file EduBase_LCD.c
 *
 * @brief Source code for the EduBase_LCD driver.
 *
 * This file contains the function definitions for the EduBase_LCD driver.
 * It interfaces with the EduBase Board 16x2 Liquid Crystal Display (LCD). 
 * The following pins are used:
 *  - Data Pin 4      [D4]  (PA2)
 *  - Data Pin 5      [D5]  (PA3)
 *  - Data Pin 6      [D6]  (PA4)
 *  - Data Pin 7      [D7]  (PA5)
 *	- LCD Enable      [E]   (PC6)
 *  - Register Select [RS]  (PE0)
 *
 * @note For more information regarding the LCD, refer to the HD44780 LCD Controller Datasheet.
 * Link: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 * @author Aaron Nanas
 */
 
#include "EduBase_LCD.h"

static uint8_t display_control = 0x00;
static uint8_t display_mode = 0x00;

void EduBase_LCD_Ports_Init(void)
{
	//Enable the clock to Port A by setting the
	//R0 bit (Bit 0) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x01;
	
	//Configure the PA5, PA4, PA3, and PA2 pins as output
	//by setting Bits 5 to 2 in the DIR register
	GPIOA->DIR |= 0x3C;
	
	//Configure the PA5, PA4, PA3, and PA2 pins to function as
	//GPIO pins by clearing Bits 5 to 2 in the AFSEL register
	GPIOA->AFSEL &= ~0x3C;
	
	//Enable the digital functionality for the PA5, PA4, PA3, and PA2 pins
	//by setting Bits 5 to 2 in the DEN register
	GPIOA->DEN |= 0x3C;
	
	//Initialize the output of the PA5, PA4, PA3, and PA2 pins to zero
	//by clearing Bits 5 to 2 in the DATA register
	GPIOA->DATA &= ~0x3C;
	
	//Enable the clock to Port C by setting the
	//R2 bit(Bit 2) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x04;
	
	//Configure the PC6 pin as output by setting Bit 6 in the DIR register
	GPIOC->DIR |= 0x40;
	
	//Configure the PC6 pin to function as a GPIO pin
	//by clearing Bit 6 in the AFSEL register
	GPIOC->AFSEL &= ~0x40;
	
	//Enable the digital functionality for the PA6 pin
	//by setting Bit 6 in the DEN register
	GPIOC->DEN |= 0x40;
	
	//Initialize the output of the PC6 pin
	//by clearing Bits 6 in the DATA register
	GPIOC->DATA &= ~0x40;
	
	//Enable the clock to Port E by setting the
	//R4 bit (Bit 4) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x10;
	
	//Configure the PE0 pin as output by setting Bit 0 in the DIR register
	GPIOE->DIR |= 0x01;
	
	//Configure the PE0 pin to function as a GPIO pin\
	//by clearing Bit 0 in the AFSEL register
	GPIOE->AFSEL &= ~0x01;
	
	//Enable the digital functionality for the PE0 pin
	//by setting Bit 0 in the DEN register
	GPIOE->DEN |= 0x01;
	
	//Initialize the output of the PE0 pin to zero
	//by clearing Bit 0 in the DATA register
	GPIOE->DATA &= ~0x01;
}

void EduBase_LCD_Pulse_Enable(void)
{
 //Ensure that the output of the PC6 pin is zero before sending a short pulse
	GPIOC->DATA &= ~0x40;
	SysTick_Delay1us(1);
	
	//Output a short pulse on the PC6 pin by setting Bit 6
	//in the DATA register high and clearing it after 1us.
	//The minimum time for the enable pulse width must be at least greater than 420 ns
	//during a read/write operation 
	GPIOC->DATA |= 0x40;
	SysTick_Delay1us(1);
	GPIOC->DATA &= ~0x40;
}

void EduBase_LCD_Write_4_Bits(uint8_t data, uint8_t control_flag)
{
 //Set the upper nibble of the data on the data pins (PA2 - PA5)
	GPIOA->DATA |= (data & 0xF0) >> 0x2;
	
	//Set or clear the register select (RS) pin based on the control flag
	//0 for command and 1 for data
	if (control_flag & 0x01)
	{
		GPIOE->DATA |= 0x01;
	}
	
	else
	{
		GPIOE->DATA &= ~0x01;
	}
	
	//Output a short pulse on the PC6 pin to enable the LCD
	EduBase_LCD_Pulse_Enable();
	
	//Clear the LCD data lines (PA2 - PA5) and provide a 1 ms delay
	GPIOA->DATA &= ~0x3C;
	SysTick_Delay1us(1000);
}

void EduBase_LCD_Send_Command(uint8_t command)
{
	//Transmit the upper nibble of the command byte
	EduBase_LCD_Write_4_Bits(command & 0xF0, SEND_COMMAND_FLAG);
	
	//Transmit the lower nibble of the command byte
	EduBase_LCD_Write_4_Bits(command << 0x4, SEND_COMMAND_FLAG);
	
	//Provide a delay based on the transmitted command
	//The first two commands require 1.52 ms execution time
	//while the rest of the commands need 37 us
	if (command < 3)
	{
		SysTick_Delay1us(1520);
	}
	else
	{
		SysTick_Delay1us(37);
	}
}

void EduBase_LCD_Send_Data(uint8_t data)
{
 //Transmit the upper nibble of the data byte
	EduBase_LCD_Write_4_Bits(data & 0xF0, SEND_DATA_FLAG);
	
	//Transmit the lower nibble of the data byte
	EduBase_LCD_Write_4_Bits(data << 0x4, SEND_DATA_FLAG);
}

void EduBase_LCD_Init(void)
{
	//Initialize the GPIO pins used by the LCD
	EduBase_LCD_Ports_Init();
	
	//Provide a delay of 50 ms after the LCD is powered on
	SysTick_Delay1us(50000);
	
	//Transmit function set initialization commands as part of the LCD initialization sequence
	EduBase_LCD_Write_4_Bits(FUNCTION_SET | CONFIG_EIGHT_BIT_MODE, SEND_COMMAND_FLAG);
	SysTick_Delay1us(4500);
	
	//This was the problem
	EduBase_LCD_Write_4_Bits(FUNCTION_SET | CONFIG_EIGHT_BIT_MODE, SEND_COMMAND_FLAG);
	SysTick_Delay1us(4500);
	

	EduBase_LCD_Write_4_Bits(FUNCTION_SET | CONFIG_EIGHT_BIT_MODE, SEND_COMMAND_FLAG);
	SysTick_Delay1us(150);
	
	//Transmit a Function Set command to the LCD to configure it to use 4-bit mode
	EduBase_LCD_Write_4_Bits(FUNCTION_SET | CONFIG_FOUR_BIT_MODE, SEND_COMMAND_FLAG);
	
	//Configure the LCD to use 5x8 dots and two rows
	EduBase_LCD_Send_Command(FUNCTION_SET | CONFIG_5x8_DOTS | CONFIG_TWO_LINES);
	
	//Transmit a Display Control command to enable the display of the LCD
	EduBase_LCD_Enable_Display();
	
	//Transmit a Clear Display command to clear the display and set the DDRAM address to 0
	EduBase_LCD_Clear_Display();
	
}

void EduBase_LCD_Clear_Display(void)
{
	EduBase_LCD_Send_Command(CLEAR_DISPLAY);
}

void EduBase_LCD_Disable_Display(void)
{
	display_control = display_control & ~(DISPLAY_ON);
	EduBase_LCD_Send_Command(DISPLAY_CONTROL | display_control);
}

void EduBase_LCD_Enable_Display(void)
{
	display_control = display_control | DISPLAY_ON;
	EduBase_LCD_Send_Command(DISPLAY_CONTROL | display_control);
}

void EduBase_LCD_Display_String(char* string)
{
	for (unsigned int i = 0; i < strlen(string); i++)
	{
		EduBase_LCD_Send_Data(string[i]);
	}
}