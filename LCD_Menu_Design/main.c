/**
#include "TM4C123GH6PM.h"
#include "PMOD_BTN_Interrupt.h"
#include "SysTick_Delay.h"
#include "EduBase_LCD.h"

//Declare the user-defined function prototype for PMOD_BTN_Interrupt
void PMOD_BTN_Handler(uint8_t pmod_btn_status);

//Declare the user-defined function prototype for EduBase_Button_Interrupt
void EduBase_Button_Handler(uint8_t edubase_button_status);

//Initialize a global variable for an 8-bit counter
static uint8_t counter = 0;

static uint8_t milliseconds = 0;
static uint8_t seconds = 0;
static uint8_t minutes = 0;

int main(void)
{
	//Initialize the push buttons on the PMOD BTN module (Port A)
	PMOD_BTN_Interrupt_Init(&PMOD_BTN_Handler);
	
	//Initialize the SysTick timer used to provide blocking delay function
	SysTick_Delay_Init();
	
	
}

void PMOD_BTN_Handler(uint8_t pmod_btn_status)
{
	switch(pmod_btn_status)
	{
		//BTN0 (PA2) is pressed
		case 0x04:
		{
			
			break;
		}
		
		//BTN1 (PA3) is pressed
		case 0x08:
		{
			
			break;
		}
		
		//BTN2 (PA4) is pressed
		case 0x10:
		{
			
			break;
		}
		
		//BTN3 (PA5) is pressed
		case 0x20:
		{
			
			break;
		}
		
		default:
		{
			
			break;
		}
	}
}
*/

/**
#include "TM4C123GH6PM.h"
#include "PMOD_BTN_Interrupt.h"
#include "SysTick_Delay.h"
#include "EduBase_LCD.h"
#include "MorseDecoder.h"

// SysTick delay initialization
void SysTick_Delay_Init(void) {
    SysTick->LOAD = 16000 - 1;  // Load value for 1 ms delay (16 MHz clock)
    SysTick->VAL = 0;           // Clear current value
    SysTick->CTRL = 0x5;        // Enable SysTick with system clock
}

// SysTick delay function
void SysTick_Delay(uint32_t delay_ms) {
    for (uint32_t i = 0; i < delay_ms; i++) {
        while ((SysTick->CTRL & 0x10000) == 0);  // Wait for COUNT flag
    }
}

// Global variables for time tracking
static uint32_t last_press_time = 0;
static uint32_t current_time = 0;

// Timing thresholds for Morse decoding (in ms)
#define DOT_THRESHOLD 200
#define DASH_THRESHOLD 600
#define CHAR_PAUSE 800
#define WORD_PAUSE 2000

// PMOD Button interrupt handler
void PMOD_BTN_Handler(uint8_t pmod_btn_status) {
    current_time = SysTick->VAL;  // Get current SysTick value

    switch (pmod_btn_status) {
        // BTN0 (PA2) is pressed - Add a dot (.)
        case 0x04:
            MorseDecoder_AddSymbol('.');
            last_press_time = current_time;
            break;

        // BTN1 (PA3) is pressed - Add a dash (-)
        case 0x08:
            MorseDecoder_AddSymbol('-');
            last_press_time = current_time;
            break;

        // BTN2 (PA4) is pressed - Decode and display the current input
        case 0x10: {
            char decoded_char = MorseDecoder_Decode();
            EduBase_LCD_Clear_Display();
            EduBase_LCD_PrintChar(decoded_char);
            break;
        }

        // BTN3 (PA5) is pressed - Clear the input buffer
        case 0x20:
            MorseDecoder_Clear();
            EduBase_LCD_Clear_Display();
            break;
    }
}

int main(void) {
    // Initialize the PMOD BTN module (Port A)
    PMOD_BTN_Interrupt_Init(&PMOD_BTN_Handler);

    // Initialize SysTick
    SysTick_Delay_Init();

    // Initialize the LCD
    EduBase_LCD_Init();
    EduBase_LCD_Clear_Display();
    EduBase_LCD_PrintString("Morse Decoder");

    while (1) {
        // Monitor time for pauses
        uint32_t elapsed_time = current_time - last_press_time;

        if (elapsed_time > CHAR_PAUSE) {
            char decoded_char = MorseDecoder_Decode();
            EduBase_LCD_Clear_Display();
            EduBase_LCD_PrintChar(decoded_char);
        }
    }
}
*/

#include "TM4C123GH6PM.h"
#include "PMOD_BTN_Interrupt.h"
#include "SysTick_Delay.h"
#include "EduBase_LCD.h"
#include "MorseDecoder.h"

// Define Morse timing thresholds (in ms)
#define DOT_THRESHOLD 200
#define DASH_THRESHOLD 600
#define CHAR_PAUSE 800
#define WORD_PAUSE 2000

// Global variables for timing
static uint32_t last_press_time = 0;

// PMOD_BTN interrupt handler
void PMOD_BTN_Handler(uint8_t pmod_btn_status) 
{
    uint32_t current_time = SysTick->VAL; // Read the current SysTick value
    uint32_t elapsed_time = current_time - last_press_time;
    
    switch (pmod_btn_status) {
			// BTN0 (PA2) pressed - Dot
        case 0x04: 
				{
						MorseDecoder_AddSymbol('.');
           // EduBase_LCD_Clear_Display();
           // EduBase_LCD_Display_String("Dot added: .");
           // last_press_time = current_time;
            break;
				}
				
        case 0x08: // BTN1 (PA3) pressed - Dash
				{
            MorseDecoder_AddSymbol('-');
           // EduBase_LCD_Clear_Display();
           // EduBase_LCD_Display_String("Dash added: -");
           // last_press_time = current_time;
            break;
				}
				
        case 0x10: // BTN2 (PA4) pressed - Decode Morse
				{
            if (elapsed_time > CHAR_PAUSE) {
                char decoded_char = MorseDecoder_Decode();
             //   EduBase_LCD_Clear_Display();
             // EduBase_LCD_Display_String("Decoded: ");
                EduBase_LCD_Send_Data(decoded_char);
            }
            //last_press_time = current_time;
            break;
				}
				
        case 0x20: // BTN3 (PA5) pressed - Clear
				{
            EduBase_LCD_Clear_Display();
         //   EduBase_LCD_Display_String("Cleared");
          //  last_press_time = current_time;
            break;
				}
				
        default:
				{
            break;
				}
    }
}

int main(void) {
    // Initialize hardware components
    SysTick_Delay_Init();
    EduBase_LCD_Init();
    PMOD_BTN_Interrupt_Init(&PMOD_BTN_Handler);
    
    // Display welcome message
		// EduBase_LCD_Display_String("Morse Decoder Ready");
    EduBase_LCD_Clear_Display();
    

    // Infinite loop
    while (1) {
        // Idle loop for handling asynchronous events
    }
}