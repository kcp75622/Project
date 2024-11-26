/**
 * @file EduBase_LCD.h
 *
 * @brief Header file for the EduBase_LCD driver.
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

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include <string.h>
#include <stdio.h>

enum LCD_Commands
{
	CLEAR_DISPLAY         	= 0x01,
	RETURN_HOME             = 0x02,
	ENTRY_MODE_SET          = 0x04,
	DISPLAY_CONTROL         = 0x08,
	CURSOR_OR_DISPLAY_SHIFT =	0x10,
	FUNCTION_SET            =	0x20,
	SET_CGRAM_ADDR          =	0x40,
	SET_DDRAM_ADDR          =	0x80
};

enum Entry_Mode_Bits
{
	ENTRY_SHIFT_DISABLE     =	0x00,
	ENTRY_SHIFT_ENABLE      =	0x01,
	ENTRY_SHIFT_DECREMENT   = 0x00,
	ENTRY_SHIFT_INCREMENT   = 0x02
};

enum Display_Control_Bits
{             
	DISPLAY_OFF             = 0x00,
	DISPLAY_ON              = 0x04
};

enum Function_Set_Bits
{
	CONFIG_5x8_DOTS         = 0x00,
	CONFIG_5x10_DOTS        = 0x04,
	CONFIG_FOUR_BIT_MODE    = 0x00,
	CONFIG_EIGHT_BIT_MODE   = 0x10,
	CONFIG_ONE_LINE         = 0x00,
	CONFIG_TWO_LINES        = 0x08
};

enum LCD_Register_Select_Flags
{
	SEND_COMMAND_FLAG       = 0x00,
	SEND_DATA_FLAG          = 0x01
};

/**
 * @brief Initializes the GPIO pins used by the 16x2 LCD on the EduBase board.
 *
 * This function initializes the following output GPIO pins used by the 
 * 16x2 Liquid Crystal Display (LCD) on the EduBase board.
 *  - Data Pin 4      [D4]  (PA2)
 *  - Data Pin 5      [D5]  (PA3)
 *  - Data Pin 6      [D6]  (PA4)
 *  - Data Pin 7      [D7]  (PA5)
 *	- LCD Enable      [E]   (PC6)
 *  - Register Select [RS]  (PE0)
 *
 * @param None
 *
 * @return None
 */
void EduBase_LCD_Ports_Init(void);

/**
 * @brief Generates a short pulse on the LCD enable pin to initiate data transmission.
 *
 * This function generates a short pulse on the LCD enable pin (PC6) to initiate
 * data transmission to the 16x2 Liquid Crystal Display (LCD) on the EduBase board.
 * A minimum pulse width greater than 450 nanoseconds is provided as specified in the datasheet.
 *
 * @param None
 *
 * @return None
 */
void EduBase_LCD_Pulse_Enable(void);

/**
 * @brief Transmits a 4-bit data at a time to the LCD.
 *
 * This function transmits a 4-bit data at a time to the LCD. It takes an 8-bit data input 
 * and extracts the upper nibble, which is then shifted to align with the pins connected 
 * to the LCD's data lines (PA2 - PA5). The control flag determines whether the operation is a data write
 * or a command write. After setting the data lines and control pin accordingly, it pulses 
 * the LCD enable pin to signal the LCD to latch in the data.
 *
 * @param data The 8-bit data to be sent to the LCD.
 
 * @param control_flag A flag indicating whether the operation is a data write or a command write.
 *                     Indicates a command write if cleared (0); Otherwise, it performs a data write
 *                     operation when set (1).
 *
 * @return None
 */
void EduBase_LCD_Write_4_Bits(uint8_t data, uint8_t control_flag);

/**
 * @brief Sends a command to the LCD.
 *
 * This function sends an 8-bit command to the LCD using the EduBase_LCD_Write_4_Bits function.
 * It transmits the upper nibble of the command first, and then it transmits the lower nibble. The timing 
 * of the delays after sending the command depends on the specific command being executed.
 * For the first two commands (i.e. Clear Display and Return Home), a delay of 1.52 ms is required.
 * The rest of the commands require a delay of 37 us.
 *
 * @param command The 8-bit command to be sent to the LCD.
 *
 * @return None
 *
 * @note The commands are listed on pages 24-25 of the HD44780 LCD Controller datasheet.
 */
void EduBase_LCD_Send_Command(uint8_t command);

/**
 * @brief Sends an 8-bit data byte to the LCD.
 *
 * This function sends an 8-bit data byte to the LCD using the EduBase_LCD_Write_4_Bits function.
 * It transmits the upper nibble of the command first, and then it transmits the lower nibble.
 *
 * @param data The 8-bit data byte to be sent to the LCD.
 *
 * @return None
 */
void EduBase_LCD_Send_Data(uint8_t data);

/**
 * @brief Initializes the LCD module connected to the EduBase board.
 *
 * This function initializes the LCD module by performing the following steps:
 * - Initializes the required GPIO pins for interfacing with the LCD.
 * - Waits for 50 ms to allow the LCD to power up.
 * - Sends the function commands several times as part of the LCD initialization sequence
 *   specified in pages 45-46 of the HD44780 LCD Controller datasheet.
 * - Sets up the LCD configuration
 *
 * @param None
 *
 * @return None
 */
void EduBase_LCD_Init(void);

/**
 * @brief Clears the display of the LCD.
 *
 * This function sends the Clear Display command (0x01) to the LCD to clear its display.
 *
 * @param None
 *
 * @return None
 */
void EduBase_LCD_Clear_Display(void);


/**
 * @brief Disables the display on the LCD.
 *
 * This function disables the display on the LCD by clearing the Display (D) bit.
 *
 * @param None
 *
 * @return None
 */
void EduBase_LCD_Disable_Display(void);

/**
 * @brief Enables the display on the LCD.
 *
 * This function enables the display on the LCD by setting the Display (D) bit.
 *
 * @param None
 *
 * @return None
 */
void EduBase_LCD_Enable_Display(void);

/**
 * @brief Displays a string on the LCD.
 *
 * This function displays a null-terminated string on the LCD. The string is iterated 
 * character by character until the end of the string is reached.
 *
 * @param string A char pointer that holds the address of a sequence of char values (i.e. string).
 *
 * @return None
 */
void EduBase_LCD_Display_String(char* string);