#include "MorseDecoder.h"
#include <string.h>

// Morse code lookup table
const char* morse_table[36] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---",
    "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

const char char_table[36] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// Buffer to store Morse input
static char morse_input[10] = {0};
static uint8_t morse_index = 0;

// Decode the Morse input and return the corresponding character
char MorseDecoder_Decode(void) {
    morse_input[morse_index] = '\0';  // Null-terminate the input string
    for (int i = 0; i < 36; i++) {
        if (strcmp(morse_input, morse_table[i]) == 0) {
            morse_index = 0;  // Reset the input buffer
            return char_table[i];
        }
    }
    morse_index = 0;  // Reset the input buffer
    return '?';       // Return '?' for invalid Morse input
}

// Add a symbol ('.' or '-') to the Morse input buffer
void MorseDecoder_AddSymbol(char symbol) {
    if (morse_index < sizeof(morse_input) - 1) {
        morse_input[morse_index++] = symbol;
    }
}

// Clear the Morse input buffer
void MorseDecoder_Clear(void) {
    morse_index = 0;
    morse_input[0] = '\0';
}