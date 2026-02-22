/*
 * Seven_Segment.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Bharath
 */

#ifndef INC_SEVEN_SEGMENT_H_
#define INC_SEVEN_SEGMENT_H_
#include "stm32f1xx.h"

#define DATA_PIN GPIO_PIN_15
#define CLOCK_PIN GPIO_PIN_13
#define LATCH_PIN GPIO_PIN_14
#define SHIFT_PORT GPIOB







void shiftOut(uint8_t data);
void shiftLatch();
void displayNumber(uint8_t number,uint8_t Position,uint8_t Decimal );
void Seven_SegmentInit();


#endif /* INC_SEVEN_SEGMENT_H_ */
