/*
 * Seven_Segment.c
 *
 *  Created on: Nov 26, 2023
 *      Author: Bharath
 */
#include "Seven_Segment.h"
char *myTexts[] = {"A"};

// variables
int text_index = 0, char_index = 0, adc_value, TIME_BUFFER, NEXT_BUFFER;
void shiftOut(uint8_t data)
{

	for (int i = 0; i < 8; i++) {


        if ((data & (0x80 >> i)) != 0) {
            HAL_GPIO_WritePin(SHIFT_PORT, DATA_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(SHIFT_PORT, DATA_PIN, GPIO_PIN_RESET);
        }



        HAL_GPIO_WritePin(SHIFT_PORT, CLOCK_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(SHIFT_PORT, CLOCK_PIN, GPIO_PIN_RESET);
    }
}


void shiftLatch() {
    HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_SET);

    HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_RESET);
}


void displayNumber(uint8_t number,uint8_t Position,uint8_t Decimal ) {
    // Define 7-segment display patterns for digits 0-9
    const uint8_t digitPatterns[] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111,  // 9
		0b01110111, // A 10
		0b01011110, // d 11
		0b01110001, // F 12
		0b00111000, // L 13
		0b01111001, // E 14
		0b00111110, // U 15
		0b01110110, // H 16
		0b01000000, // - 17
		0b00000000, // CLEAR 18
    };
    const uint8_t PositionPatterns[] = {
            0x01,0X02,0X04,0X08,0x00,0X10,0x20,0x40,0x30
        };
    /*if (number >= 0 && number <= 9) {
        shiftOut(digitPatterns[number]);
       shiftLatch();
    }*/

    HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_SET);
    shiftOut(PositionPatterns[Position]);
if(Decimal == 1){
    shiftOut(digitPatterns[number]|0b10000000);
}
else
	shiftOut(digitPatterns[number]);

    HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_RESET);


}

void Seven_SegmentInit()
   {
     // Configure the GPIO pins for Data, Clock, and Latch as output
		 GPIO_InitTypeDef GPIO_InitStruct;
		 __HAL_RCC_GPIOB_CLK_ENABLE();

		 GPIO_InitStruct.Pin = DATA_PIN | CLOCK_PIN | LATCH_PIN;
		 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		 GPIO_InitStruct.Pull = GPIO_NOPULL;
		 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		 HAL_GPIO_Init(SHIFT_PORT, &GPIO_InitStruct);
    }

unsigned char convertCharacterToPattern (char ASCII) {

    // what is the character?
    switch (ASCII) {

        // numbers         abcdefg.
        case '0': return 0b11111100;
        case '1': return 0b01100000;
        case '2': return 0b11011010;
        case '3': return 0b11110010;
        case '4': return 0b01100110;
        case '5': return 0b10110110;
        case '6': return 0b10111110;
        case '7': return 0b11100000;
        case '8': return 0b11111110;
        case '9': return 0b11110110;

        // letters         abcdefg.
        case 'a': return 0b11101110;

        case 'A': return 0b11111100;
        case 'b': return 0b00111110;
        case 'B': return 0b00111110;
        case 'c': return 0b00011010;
        case 'C': return 0b10011100;
        case 'd': return 0b01111010;
        case 'D': return 0b01111010;
        case 'e': return 0b10011110;
        case 'E': return 0b10011110;
        case 'f': return 0b10001110;
        case 'F': return 0b10001110;
        case 'g': return 0b10111100;
        case 'G': return 0b10111100;
        case 'h': return 0b00101110;
        case 'H': return 0b01101110;
        case 'i': return 0b00100000;
        case 'I': return 0b01100000;
        case 'j': return 0b01111000;
        case 'J': return 0b11111000;
        case 'k': return 0b10101110;
        case 'K': return 0b10101110;
        case 'l': return 0b00111100;
        case 'L': return 0b00011100;
        case 'm': return 0b10101000;
        case 'M': return 0b10101000;
        case 'n': return 0b00101010;
        case 'N': return 0b11101100;
        case 'o': return 0b00111010;
        case 'O': return 0b11111100;
        case 'p': return 0b11001110;
        case 'P': return 0b11001110;
        case 'q': return 0b11100110;
        case 'Q': return 0b11100110;
        case 'r': return 0b00001010;
        case 'R': return 0b00001010;
        case 's': return 0b10110110;
        case 'S': return 0b10110110;
        case 't': return 0b00011110;
        case 'T': return 0b00011110;
        case 'u': return 0b00111000;
        case 'U': return 0b01111100;
        case 'v': return 0b00111000;
        case 'V': return 0b01111100;
        case 'w': return 0b01010100;
        case 'W': return 0b01010100;
        case 'x': return 0b01101110;
        case 'X': return 0b01101110;
        case 'y': return 0b01100110;
        case 'Y': return 0b01110110;
        case 'z': return 0b11011010;
        case 'Z': return 0b11011010;

        // symbols          abcdefg.
        case '=':  return 0b00010010;
        case '-':  return 0b00000010;
        case '_':  return 0b00010000;
        case 248:  return 0b11000110; // degree sign
        case '\'': return 0b01000000;
        case '"':  return 0b01000100;
        case '(':  return 0b10011100;
        case '[':  return 0b10011100;
        case '{':  return 0b10011100;
        case ')':  return 0b11110000;
        case ']':  return 0b11110000;
        case '}':  return 0b11110000;
        case '.':  return 0b00000001;
        case ',':  return 0b00100000;
        case ';':  return 0b00100001;
        case '!':  return 0b01000001;
        case '?':  return 0b11001011;
        case ' ':  return 0b00000000;

        // default symbol is an empty space
        default: return 0b00000000;

    }

}

void Gas_IndexMenu(){  //collect the data from sensor store in it buffer

	    displayNumber(14,0,1);//e
		HAL_Delay(2);
		displayNumber(13,1,1);//l
		HAL_Delay(2);
		displayNumber(9,2,1);
		HAL_Delay(2);
		displayNumber(9,3,0);
		HAL_Delay(2);
}
void Range_IndexMenu(){//collect the data from array buffer

	        displayNumber(1,0,0);
			HAL_Delay(2);
			displayNumber(2,1,0);
			HAL_Delay(2);
			displayNumber(3,2,0);
			HAL_Delay(2);
			displayNumber(4,3,0);
			HAL_Delay(2);
}
void Version_Info(){//collect the data from array buffer

	displayNumber(15,0,1);
	HAL_Delay(2);
	displayNumber(1,1,1);
	HAL_Delay(2);
	displayNumber(0,2,1);
	HAL_Delay(2);
	displayNumber(2,3,0);
	HAL_Delay(2);
}
void HighLow_AlarmMenu(){//collect the data from array buffer
	            displayNumber(16,0,1);
				HAL_Delay(2);
				displayNumber(13,1,0);
				HAL_Delay(2);
				displayNumber(5,2,0);
				HAL_Delay(2);
				displayNumber(5,3,0);
				HAL_Delay(2);

}
void Default_Address(){//collect the data from array buffer
			displayNumber(11,0,1);
			HAL_Delay(2);
			displayNumber(2,1,0);
			HAL_Delay(2);
			displayNumber(5,2,0);
			HAL_Delay(2);
			displayNumber(5,3,0);
			HAL_Delay(2);
}

/// selection menu
void Address_Menu(){//collect the data from array buffer
			displayNumber(17,0,0);
			HAL_Delay(2);
			displayNumber(17,1,0);
			HAL_Delay(2);
			displayNumber(10,2,0);
			HAL_Delay(2);
			displayNumber(11,3,0);
			HAL_Delay(2);
}

void zero_calibartionMenu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(10,2,0);
		HAL_Delay(2);
		displayNumber(0,3,0);
		HAL_Delay(2);

}
void gas_calibartionMenu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(10,2,0);
		HAL_Delay(2);
		displayNumber(12,3,0);
		HAL_Delay(2);

}
void Dac_calibartionMenu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(10,2,0);
		HAL_Delay(2);
		displayNumber(2,3,0);
		HAL_Delay(2);

}
void AlarmLimitMenu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(10,2,0);
		HAL_Delay(2);
		displayNumber(13,3,0);
		HAL_Delay(2);

}
void h_AlarmLimitMenu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(10,2,0);
		HAL_Delay(2);
		displayNumber(16,3,0);
		HAL_Delay(2);

}
void LifeLimitMenu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(14,2,0);
		HAL_Delay(2);
		displayNumber(13,3,0);
		HAL_Delay(2);

}
void Self_calibartionMenu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(14,2,0);
		HAL_Delay(2);
		displayNumber(0,3,0);
		HAL_Delay(2);

}
void Exit_Menu(){
	displayNumber(17,0,0);
		HAL_Delay(2);
		displayNumber(17,1,0);
		HAL_Delay(2);
		displayNumber(17,2,0);
		HAL_Delay(2);
		displayNumber(17,3,0);
		HAL_Delay(2);

}

void Number_Disp(uint8_t number ) {
    // Define 7-segment display patterns for digits 0-9
    const uint8_t digitPatterns[] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111,  // 9

    };

    //HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_SET);


	shiftOut(digitPatterns[number]);

    HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_RESET);


}

void Segment_Selector(uint8_t Position)
{

	const uint8_t PositionPatterns[] = {
            0xF1,0XF2,0XF4,0XF8,0x00,0X10
        };
		HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_SET);
		shiftOut(PositionPatterns[Position]);
	   // HAL_GPIO_WritePin(SHIFT_PORT, LATCH_PIN, GPIO_PIN_RESET);

}
