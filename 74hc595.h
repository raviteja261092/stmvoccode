#ifndef __74HC595_h__
#define __74HC595_h__


#include "stm32f1xx.h"




#define clockPin       GPIO_PIN_13
#define serialDataPin  GPIO_PIN_15
#define latchPin       GPIO_PIN_14
#define ShiftRegister74HC595_GPIO_Port GPIOB
#define Number_of_Registers    0

typedef enum
{
  LOW595 = 0,
  HIGH595
}PinState595;

typedef enum
{
  LOW = 0,
  HIGH
}bool;

extern unsigned char  shiftRegisters[];

void SerialDataPinSet(PinState595 state);
void ClockPinSet(PinState595 state);
void LatchPinSet(PinState595 state);
void ShiftRegister74HC595_clear(void);
void ShiftRegister74HC595_setAll(bool val);
void ShiftRegister74HC595_setPin(int index, bool val);
void ShiftRegister74HC595_update(void);
void ShiftRegister74HC595_init(void);
#endif
