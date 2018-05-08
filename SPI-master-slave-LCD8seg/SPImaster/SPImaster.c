/*
 * SPImaster.c
 *
 *  Author: Stefoo
 */ 


#include <avr\io.h>
#include <avr\interrupt.h>
#include <util\delay.h>
#include <string.h>
#include <stdio.h>
#include "multi.h"

static inline void SPI_Set_SS()
{
	PORTB |= _BV(PB2);
}

static inline void SPI_Reset_SS()
{
	PORTB &= ~(_BV(PB2));
}

void SPI_master_init()
{
	SPI_Set_SS();
	DDRB |= (_BV(PB2) | _BV(PB3) | _BV(PB5));	//Piny SS, MOSI, SCK jako wyjscie
	SPCR = _BV(SPE) | _BV(MSTR);	// Tryb master, CLK/4
	SPSR;
	SPDR;	// Skasuj flage SPIF (odczyt SPI Status Register oraz SPI Data Register)
}

uint8_t SPI_send_rec_byte(uint8_t byte)
{
	SPDR = byte;
	while(!(SPSR & _BV(SPIF)));
	return SPDR;
}

uint16_t GetTemperature(uint8_t LMno)
{
	uint16_t temperature;
	SPI_Reset_SS();
	SPI_send_rec_byte(LMno);	// Odbierany bajt jest ignorowany
	_delay_us(100);
	temperature = SPI_send_rec_byte(LMno);	// Wysylany bajt jest ignorowany
	_delay_us(10);
	temperature = 256*SPI_send_rec_byte(LMno) + temperature;	// Wysylany bajt jest ignorowany
	SPI_Set_SS();
	return temperature;
}

int main()
{
	char wynik[7];

	SPI_master_init();
	Timer0Init();
	sei();
	
	while(1)
	{
		for(uint8_t i=0;i<2;i++)
		{
			sprintf(wynik, "%04d", GetTemperature(i));
			for(uint8_t i = 0; i < 4; i++) wynik[i] -= '0';	// konwersja znak_ASCII -> cyfra
			memcpy((char*)LEDDIGITS, (char*)wynik, LEDDISPNO);
			_delay_ms(1000);
		}
	}
}
