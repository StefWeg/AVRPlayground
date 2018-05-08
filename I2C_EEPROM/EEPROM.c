/*
 * EEPROM.c
 *
 *  Author: Stefoo
 */ 


#include <avr/io.h>
#include <string.h>
#include <stddef.h>
#include "I2cbase.h"

#include <util\delay.h>


#define I2C_MEM_PAGE_SIZE 64	// Wielkosc strony pamieci EEPROM

void I2C_MEM_SetRWAddress(const __uint24 addr)
{
	do		// Wybierz urzadzenie o wskazanym adresie
	{
		I2C_SendStartAndSelect(addr >> 16);
	} while(I2C_Error == I2C_NoACK);	// Czekaj, az urzadzenie wysle ACK

	I2C_SendByte((uint8_t)(addr >> 8));
	I2C_SendByte((uint8_t)(addr & 0xFF));
}

uint8_t I2C_MEM_read_byte(const __uint24 addr)
{
	I2C_MEM_SetRWAddress(addr);                     // Wybierz uklad i adres odczytywanej komorki
	I2C_SendStartAndSelect((addr >> 16) | TW_READ); // Wykonaj operacje Current Address Read
	uint8_t byte = I2C_ReceiveData_NACK();
	I2C_Stop();
	return byte;
}

void I2C_MEM_write_byte(const __uint24 addr, uint8_t data)
{
	I2C_MEM_SetRWAddress(addr);
	I2C_SendByte(data);
	I2C_Stop();
	I2C_WaitTillStopWasSent();	// Poczekaj na zakonczenie wysylania sygnalu STOP
}

void I2C_MEM_read_block(void *dst, const __uint24 src, size_t len)
{
	I2C_MEM_SetRWAddress(src);
	I2C_SendStartAndSelect((src >> 16) | TW_READ);
	while(--len)
	{
		*(uint8_t*)dst = I2C_ReceiveData_ACK();
		dst = ((uint8_t*)dst)+1;
	}
	*(uint8_t*)dst = I2C_ReceiveData_NACK();
	// Ostatni odczytywany bajt nie moze zostac potwierdzony, co konczy operacje odczytu
	I2C_Stop();
}

void I2C_MEM_write_block(const void *src, const __uint24 dst, size_t len)
{
	size_t ps;
	size_t adres = (uint16_t)dst;

	do
	{
		I2C_MEM_SetRWAddress(dst);
		ps = I2C_MEM_PAGE_SIZE - (adres % I2C_MEM_PAGE_SIZE);	// Oblicz liczbe bajtów do konca strony pamieci
		adres += ps;		// Adres kolejnej strony pamiêci
		if(ps > len) ps = len;
		len -= ps;
		while(ps--)
		{
			I2C_SendByte(*(uint8_t*)src);
			src = ((uint8_t*)src)+1;
		}
		I2C_Stop();
		I2C_WaitTillStopWasSent();	// Zaczekaj do konca operacji wysylania danych
	} while(len);
}

#define SIZEOF(s,m) ((size_t) sizeof(((s *)0)->m))

#define I2C_MEM_write_var(var, EEvarname) \
{       \
	__uint24 _addr;   \
	_addr = 0xA00000UL;  \
	_addr |= offsetof(I2CEEPROM, EEvarname); \
	I2C_MEM_write_block(&var, _addr, SIZEOF(I2CEEPROM, EEvarname)); \
}

#define I2C_MEM_read_var(var, EEvarname) \
{       \
	__uint24 _addr;   \
	_addr = 0xA00000UL;  \
	_addr |= offsetof(I2CEEPROM, EEvarname); \
	I2C_MEM_read_block(&var, _addr, SIZEOF(I2CEEPROM, EEvarname)); \
}

int main(void)
{
	I2C_Init();   //Zainicjuj interfejs TWI
	
	
	__uint24 addr = 0xA00000UL;  // 0xA0 == 0b10100000
	/*
	 I2C_MEM_write_byte(addr,0x33);
	 addr++;
	 I2C_MEM_write_byte(addr,0x44);
	 addr++;
	 I2C_MEM_write_byte(addr,0x55);

	 addr = 0xA00000UL;
	 volatile uint8_t dana1 = I2C_MEM_read_byte(addr);
	 addr++;
	 volatile uint8_t dana2 = I2C_MEM_read_byte(addr);
	 addr++;
	 volatile uint8_t dana3 = I2C_MEM_read_byte(addr);
	 */
	
	typedef struct
	{
		uint16_t zmienna1;
		uint8_t zmienna2;
		char zmienna3[10];
	} I2CEEPROM;
	
	I2CEEPROM* const pEE = 0x0000;
	volatile uint16_t dana16 = 0x4321;

	/*
	// addr = 0xA00000 | (uint16_t)&pEE->zmienna1;
	addr = 0xA00000 | offsetof(I2CEEPROM, zmienna1);
	I2C_MEM_write_block(&dana16, addr, sizeof(pEE->zmienna1));
	*/
	
	I2C_MEM_write_var(dana16, zmienna1);
	volatile uint16_t odebrana16 = 0;
	I2C_MEM_read_var(odebrana16, zmienna1);

	volatile char struna[10] = {'s','i','e','m','a','n','e','r','k','a'};
	I2C_MEM_write_var(struna[0], zmienna3);
	volatile char struna2[10];
	I2C_MEM_read_var(struna2[0], zmienna3);

	// WORKIN'!

	DDRD = 0x01;
	PORTD = 0xFE;
	
    while(1)
    {
        PORTD ^= 0x01;
        _delay_ms(1000);
    }
}
