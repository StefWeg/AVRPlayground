/*
 * PCF8574N_custom.c
 *
 * Created: 07.09.2017 16:19:16
 * Author : Stefoo
 */ 

#include <avr\io.h>
#include <stdint.h>
#include <util\delay.h>
#include "I2CBase.h"

#define PCF8574DDR 0x40

void I2C_StartSelectWait(uint8_t addr)
{
	do
	{
		I2C_SendStartAndSelect(addr);
	} while(I2C_Error == I2C_NoACK);
}

int main()
{
	uint8_t x = 0xFF;
	uint8_t i = 0;
	volatile uint8_t zmienna = 0;
	I2C_Init();

	I2C_StartSelectWait(PCF8574DDR);
	while(1)
	{
		x ^= _BV(i);
		I2C_SendByte(x);

		// I2C_SendByte(0xFF ^ (1 << i));

		i = (i+1)%8;
		_delay_ms(100);
	}
	I2C_Stop();
}

