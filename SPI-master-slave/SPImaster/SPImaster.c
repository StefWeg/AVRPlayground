/*
 * SPImaster.c
 *
 *  Author: Stefoo
 */ 


#include <avr\io.h>
#include <util\delay.h>
#include <string.h>
#include <stdio.h>
#include "KS0108.h"
#include "graphics.h"

static inline void SPI_Set_SS()
{
	PORTB|=_BV(PB2);
}

static inline void SPI_Reset_SS()
{
	PORTB&=~(_BV(PB2));
}

void SPI_master_init()
{
	SPI_Set_SS();
	DDRB|=(_BV(PB2) | _BV(PB3) | _BV(PB5));	//Piny SS, MOSI, SCK jako wyjœcie
	SPCR=_BV(SPE) | _BV(MSTR); //Tryb master, CLK/4
	SPSR;
	SPDR; //Skasuj flagê SPIF
}

uint8_t SPI_send_rec_byte(uint8_t byte)
{
	SPDR=byte;
	while(!(SPSR & _BV(SPIF)));
	return SPDR;
}

uint16_t GetTemperature(uint8_t LMno)
{
	uint16_t temperature;
	SPI_Reset_SS();
	SPI_send_rec_byte(LMno);
	_delay_us(100);
	temperature=SPI_send_rec_byte(LMno);
	_delay_us(10);
	temperature=256*SPI_send_rec_byte(LMno)+temperature;
	SPI_Set_SS();
	return temperature;
}

int main()
{
	char wynik[7];

	GLCD_init();
	GLCD_cls();
	GLCD_goto(0,0);
	SPI_master_init();

	GLCD_puttext(PSTR("Temperatura:"));
	GLCD_goto(0,1);
	GLCD_puttext(PSTR("T1:"));
	GLCD_goto(0,2);
	GLCD_puttext(PSTR("T2:"));
	
	while(1)
	{
		for(uint8_t i=0;i<2;i++)
		{
			sprintf(wynik, "%5d", GetTemperature(i));
			uint8_t len=strlen(wynik);
			memmove(&wynik[len-1], &wynik[len-2], 3);
			wynik[len-2]=',';
			GLCD_goto(15, i+1);
			GLCD_puttext(wynik);
		}
		_delay_ms(500);
	}
}
